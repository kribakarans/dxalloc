
#ifndef __DXALLOC_H__
#define __DXALLOC_H__

#include <stdio.h>
#include <stdlib.h>

/*
  The fclose and free APIs are implemented as macro functions that
  preserve user pointer as NULL in order to prevent dangling pointers
 */
#define dxfclose(__dxfp) \
    do { \
        if (__dxfp != NULL) { \
            fclose (__dxfp); \
            dxt_pop_trace(__dxfp); \
            __dxfp = NULL; \
        } \
    } while(0)

#define dxfree(__dxptr) \
    do { \
        if (__dxptr != NULL) { \
            free (__dxptr); \
            dxt_pop_trace(__dxptr); \
            __dxptr = NULL; \
        } \
    } while(0)

/* malloc apis */
void *dxmalloc (size_t size);
void *dxcalloc (size_t nmemb, size_t size);
void *dxrealloc (void *ptr, size_t size);

/* string apis */
char *dxstrdup (const char *str);
int dxasprintf (char **strp, const char *fmt, ...);
int dxvasprintf (char **strp, const char *fmt, va_list ap);

/* file apis */
FILE *dxfopen (const char *pathname, const char *mode);

/* dxtrace apis */
extern void init_dxtrace (void);
extern int  dxt_pop_trace (void *ptr);
extern int  dxt_push_trace (void *ptr);

#endif /*EOF */

