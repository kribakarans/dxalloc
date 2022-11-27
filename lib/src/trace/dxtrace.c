
#define _GNU_SOURCE

#include "dxtrace.h"

#include <logit/logit.h>

#include <dlfcn.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <execinfo.h>
#include <backtrace.h>

#define DXTRACE_USE_DLADDR
#ifndef DXTRACE_USE_DLADDR
	#define dxt_print_frame_worker dxt_print_frame
#else
	#define dxt_print_frame_worker dxt_print_frame_r
#endif

bool dbgdxtrace = DXTDEBUG;

dxt_t dxt;
bool traceit = 0;
static GHashTable *hashtable;
struct backtrace_state *btstate;

/* print frame details of backtrace with program counter, file, lineno and caller */
static int dxt_print_frame (void *data, uintptr_t pc, const char *file, int lineno, const char *function)
{
	if (pc != (uintptr_t) -1) { /* (pc != 0xFFFFFFFFFFFFFFFF) */
		dxinfo("  |__  0x%-13lx: %25s (in %s:%d)", pc, (function ? function : "??"), (file ? file : "??"), lineno);
	}

	return 0;
}

/* Re-entrant of dxt_print_frame(): dladdr() is called to resolve symbols        */
/* Print frame details of backtrace with program counter, file, lineno and caller */
static int dxt_print_frame_r(void *data, uintptr_t pc, const char *file, int lineno, const char *function)
{
	int       rc = -1;
	int   retval = -1;
	Dl_info info = {0};

	do {
		rc = dladdr((void*)pc, &info);
		//dxinfo(" ==> 0x%-13lx: %25s (in %s)", pc, info.dli_sname, info.dli_fname);
		if (rc != 0) {
			dxinfo_r("  |__  0x%-13lx: %25s (in %s:%d)", pc, 
			               (info.dli_sname ? info.dli_sname : (function ? function : "??")),
			               (file ? file : (info.dli_fname ? info.dli_fname : "??")), lineno);
		}

		retval = 0;
	} while(0);

	return retval;
}

static void backrace_state_error(void *data, const char *msg, int errnum)
{
	char *caller = (char *)data;

	dxerr("backtrace_create_state() failed at %s() !!! [%s/%s]", caller, msg, strerror(errno));

	return;
}

static void backtrace_error(void *data, const char *msg, int errnum)
{
	dxerr("backtrace_pcinfo() failed !!! [%s/%s]", msg, strerror(errno));

	return;
}

void dxt_print_backtrace(uintptr_t key, bt_t *bt)
{
	int i = 0;

	do {
		if ((key < (u_int32_t) -1) || (bt == NULL)) {
			dxerr("invalid arguments !!!");
			break;
		}

		dxinfo_r("Backtrace of heap-pointer: 0x%lx", key);

		if (key != bt->hptr) {
			dxerr("%s: invalid key-value pair !!! [%lx != %lx]", __func__, key, bt->hptr);
			break;
		}

		for (i = 0; i < bt->depth; i++) {
			backtrace_pcinfo(btstate, bt->pc[i], dxt_print_frame_worker, backtrace_error, NULL);
		}

		dxinfo("");
	} while(0);

	return;
}

static void lookup_hashtable(gpointer key, gpointer val, gpointer data)
{
	//dxinfo(" ===> %s: hptr: %p bt: %p", __func__, (void *) key, (void *) val);
	dxt_print_backtrace((uintptr_t)key, val);
}

/* Print final report at end */
static void print_dxtrace_report(GHashTable *ht)
{
	guint n = 0;

	dxinfo_r("\nHEAP TRACE SUMMARY:");

	n = g_hash_table_size(ht);
	if (n == 0) {
		dxinfo_r("Heap trace: All heap blocks were freed !!!\n");
	} else {
		g_hash_table_foreach(ht, lookup_hashtable, NULL);
		dxinfo_r("Heap trace: Memory leak at %u blocks !!!\n", n);
	}

	return;
}

static void dxt_atexit_handler(void)
{
	static int called = 0;

	/* prevent multiple calls */
	if (called == 1) {
		dxerr("heap-trace: summary already printed !!!");
		return;
	}

	called++;

	dxlog("hashtable: %p", (void *)hashtable);
	print_dxtrace_report(hashtable);
	g_hash_table_remove_all(hashtable);
	g_hash_table_destroy(hashtable);
	close_dxlogger();

	return;
}

void init_dxtrace(void)
{
	static int htinit = 0;

	/* prevent multiple inits */
	if (htinit == 1) {
		dxinfo("heap-trace is initiated already !!!");
		return;
	}

	htinit++;
	traceit = true;
	init_dxlogger();
	hashtable = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
	if (hashtable == NULL) {
		dxlog("failed to create Heap Table !!!");
		abort();
	}

	atexit(&dxt_atexit_handler);
	dxlog("hashtable: %p", (void *)hashtable);
	btstate = backtrace_create_state(NULL, 0, backrace_state_error, NULL);

	return;
}

/* This is a callback function called by backtrace_pcinfo() */
static int dxt_save_frame(void *data, uintptr_t pc)
{
	bt_t *bt = (bt_t *) data;

	assert(data != NULL);

	/* store program counter value */
	bt->pc[bt->depth] = pc;

	isdxlog("frame[%d]: pc: %p bt: %p hptr %p", bt->depth, (void *) bt->pc[bt->depth], (void *) bt, (void *) bt->hptr);

	/* increment stack posistion */
	bt->depth = (bt->depth + 1);

	return 0;
}

static int dxt_print_node(bt_t *bt)
{
	int i = -1;

	if (bt == NULL) {
		dxerr("bt is NULL !!!");
		return -1;
	}

	dxinfo("    ==> %s :: bt: %p hptr: %p nframes: %d", __func__, bt, (void *)bt->hptr, bt->depth);
	for (i = 0; bt->pc[i] != (uintptr_t) -1; i++) {
		dxinfo("                                             pc[%d]: %p", i, (void *)bt->pc[i]);
	}

	return 0;
}

/*
  Added stack trace entry to the hashtable
  Return -1: On failure
  Return  0: On success or key already exist
  Return  1: If table not initialized
*/
int dxt_push_trace(void *ptr)
{
	int retval = -1;
	bt_t *bt = NULL;

	do {
		if (ptr == NULL) {
			dxerr("ptr is null !!!");
			retval = -1;
			break;
		}

		if (traceit == false) {
			retval = 1;
			break;
		}

		bt = malloc(sizeof(*bt));
		if (bt == NULL) {
			dxerr("malloc() failed for bt !!!");
			retval = -1;
			break;
		}

		dxlog("ptr: %p bt: %p", ptr, bt);

		bt->depth = 0;
		bt->hptr = (uintptr_t) ptr;
		dxt.nskip = 1; /* number of toplevel frames to skip in dxt_print_backtrace output */
		if (backtrace_simple(btstate, dxt.nskip, dxt_save_frame, backtrace_error, bt) < 0) {
			dxerr("%s", "backtrace_simple failed !!!");
			retval = 1;
			break;
		}

		if (g_hash_table_contains(hashtable, ptr) != TRUE) {
			if (g_hash_table_insert(hashtable, (gpointer) ptr, (gpointer) bt) != TRUE) {
				dxerr("insert failed !!! ptr: %p bt: %p", ptr, bt);
				retval = -1;
				break;
			}
		}
		retval = 0;
	} while(0);

	return retval;
}

/*
  Remove stack trace entry from the hashtable
  Return -1: On failure
  Return  0: On success or key not found
  Return  1: If table not initialized
*/
int dxt_pop_trace(void *ptr)
{
	int retval = -1;

	do {
		if (ptr == NULL) {
			dxerr("ptr is null !!!");
			retval = -1;
			break;
		}

		if (traceit == false) {
			retval = 1;
			break;
		}

		dxlog("ptr: %p", ptr);

		if (g_hash_table_contains(hashtable, ptr) == TRUE) {
			if (g_hash_table_remove(hashtable, ptr) != TRUE) {
				dxerr("remove failed !!! ptr: %p", ptr);
				retval = -1;
				break;
			}
		}
		retval = 0;
	} while(0);

	return retval;
}

//EOF
