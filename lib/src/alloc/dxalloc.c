
#define _GNU_SOURCE

#include "dxalloc.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <logit/logit.h>

#define DXTRACE_PUSH_TRACE(__dxptr)                      \
    do {                                                 \
        if (__dxptr != NULL) {                           \
            if (dxt_push_tracepoint(__dxptr) < 0) {      \
                dxerr("dxtrace_push '%p' failed !!!",    \
                                    (void *)__dxptr);    \
                abort();                                 \
            }                                            \
        }                                                \
    } while(0);

void *dxmalloc(size_t size)
{
	void *ptr = NULL;

	ptr = malloc(size);
	if (ptr == NULL) {
		dxerr("failed: %s", strerror(errno));
		abort();
	}

	memset(ptr, 0x00, size);

	DXTRACE_PUSH_TRACE(ptr);

	return ptr;
}

void *dxcalloc(size_t nmemb, size_t size)
{
	void *ptr = NULL;

	ptr = calloc(nmemb, size);
	if (ptr == NULL) {
		dxerr("failed: %s", strerror(errno));
		abort();
	}

	DXTRACE_PUSH_TRACE(ptr);

	return ptr;
}

void *dxrealloc(void *ptr, size_t size)
{
	void *new = NULL;

	new = realloc(ptr, size);
	if (new == NULL) {
		dxerr("failed: %s", strerror(errno));
		abort();
	}

	if (ptr != new) {
		dxt_pop_tracepoint(ptr);
		DXTRACE_PUSH_TRACE(new);
	}

	return new;
}

char *dxstrdup(const char *str)
{
	size_t len = 0;
	char  *new = NULL;

	if (str == NULL) {
		dxerr("%s", "null input");
		abort();
	}

	len = strlen(str) + 1;
	new = calloc(len, sizeof(*new));
	if (new == NULL) {
		dxerr("calloc() failed: %s", strerror(errno));
		abort();
	}

	if (strcpy(new, str) == NULL) {
		dxerr("strcpy() failed: %s", strerror(errno));
		abort();
	}

	DXTRACE_PUSH_TRACE(new);

	return new;
}

int dxasprintf(char **strp, const char *fmt, ...)
{
	va_list ap;
	int ret = -1;

	va_start(ap, fmt);

	ret = vasprintf(strp, fmt, ap);
	if (ret < 0) {
		dxerr("failed: %s", strerror(errno));
		abort();
	}

	va_end(ap);

	DXTRACE_PUSH_TRACE(*strp);

	return ret;
}

int dxvasprintf(char **strp, const char *fmt, va_list ap)
{
	int ret = -1;

	ret = vasprintf(strp, fmt, ap);
	if (ret < 0) {
		dxerr("failed: %s", strerror(errno));
		abort();
	}

	DXTRACE_PUSH_TRACE(*strp);

	return ret;
}

FILE *dxfopen(const char *pathname, const char *mode)
{
	FILE *ptr = NULL;

	ptr = fopen(pathname, mode);

	DXTRACE_PUSH_TRACE(ptr);

	return ptr;
}

/* EOF */
