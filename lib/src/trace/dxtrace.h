
#ifndef __HEAPTRACE_H__
#define __HEAPTRACE_H__

#include <glib.h>
#include <stdint.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define DXTDEBUG 0
#define BTFRAME_MAX 64

typedef struct {
	pid_t pid;   /* current process id */
	int nskip;   /* number of toplevel frames to skip in dxt_print_backtrace output */
	FILE *logfp; /* log file pointer */
} dxt_t;

/* stacktrace who allocated heap memory */
typedef struct htval {
	int depth;                 /* number of tracked frames          */
	uintptr_t hptr;            /* tracked heap pointer address      */
	uintptr_t pc[BTFRAME_MAX]; /* array of tracked program counters */
} bt_t;

/* key:value pair of hash table */
typedef struct ht_node {
	uintptr_t key; /* heap pointer as key             */
	bt_t bt;       /* backtrace of respective pointer */
} node_t;

extern dxt_t dxt;

#endif /* EOF */
