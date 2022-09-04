
#include "logit.h"

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define FILENAMELEN 512
#define TMPDIR "/tmp"
#define PREFIX "" //TODO: need to handle with getenv() for Termux
#define LOGITFMT ("\33[2K\r%5d %s%20s:%-4d %27s : ")

int init_dxlogger(void)
{
	int retval = -1;
	char logfile[FILENAMELEN] = {0};

	dxt.pid = getpid();
	retval = snprintf(logfile, sizeof(logfile), "%s%s/%s", PREFIX, TMPDIR, DXTLOGFILE);

	dxt.logfp = fopen(logfile, "a+");
	if (dxt.logfp == NULL) {
		fprintf(stderr, "fopen() logfile failed !!! %s\n", strerror(errno));
		abort();
	}

	return 0;
}

int close_dxlogger(void)
{
	if (dxt.logfp != NULL) {
		fflush(dxt.logfp);
		fclose(dxt.logfp);
		dxt.logfp = NULL;
	}

	return 0;
}

void fprintdebug(FILE *fp, const char *caller, const char *file, const int line, const char *fmt, ...)
{
	va_list ap;
	time_t curtime;
	char tmbuff[64] = {0};
	struct tm *loc_time = NULL;

	if ((caller == NULL) || (file == NULL) || (fmt == NULL) || (line < 0)) {
		fprintf(stderr, "%s: invalid input arguments !!!\n", __func__);
		return;
	}

	/* init varg list */
	va_start(ap, fmt);

	/* get UTC time */
	curtime = time (NULL);

	/* Convert UTC to local time */
	loc_time = localtime (&curtime);

	/* if logfile is not initialized;
	   print debug messages to stdout */ 
	if (fp == NULL) {
		fp = stdout;
	}

	/* Prefix PID, Time, File, Line and Debugee function */
	strftime(tmbuff, sizeof tmbuff, "%H:%M:%S ", loc_time);
	fprintf(fp, LOGITFMT, getpid(), tmbuff, file, line, caller);

	/* print debugee message */
	vfprintf(fp, fmt, ap);
	fprintf(fp, "\n");
	fflush(fp);

	/* close varg list */
	va_end(ap);

	return;
}

/* EOF */
