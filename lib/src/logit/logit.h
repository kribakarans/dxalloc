
#ifndef __DEXER_LOGIT_H__
#define __DEXER_LOGIT_H__

#include <stdio.h>
#include <trace/dxtrace.h>

#define DXTLOGFILE "dxalloc.log"

#define dxlog __logit

#define dxinfo(fmt,args...) \
    do { \
        fprintf(stdout, fmt "\n", ##args); \
        fflush(stdout); \
    } while(0);

#define dxinfo_r(fmt,args...) \
    do { \
        fprintf(dxt.logfp, fmt "\n", ##args); \
        fflush(dxt.logfp); \
        dxinfo(fmt, ##args); \
    } while(0);

#define dxerr(msg...) \
    do { \
        __logit(msg); \
       fprintdebug(stderr, __FUNCTION__, __FILE__, __LINE__, msg); \
    } while(0);

#define DEBUG(x) ((dbgdxtrace == true) ? (x) : (0))
#define isdxlog(msg...)   ((dbgdxtrace == true) ? (__logit(msg)) : (0))
#define __logit(msg...  ) (fprintdebug(dxt.logfp, __FUNCTION__, __FILE__, __LINE__, msg))

int init_dxlogger(void);
int close_dxlogger(void);
void fprintdebug(FILE *fp, const char *caller, const char *file, const int line, const char *fmt, ...);

#endif

