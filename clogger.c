#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include "clogger.h"


int LogCreated = FALSE;


void Log(char *msg)
{
	FILE *file;

	if (!LogCreated) {
		file = fopen(get_filename(), "w");
		LogCreated = TRUE;
	} else {
		file = fopen(get_filename(), "a");
	}

	if (file == NULL) {
		if (LogCreated)
			LogCreated = FALSE;
		return;
	} else {
		fputs(msg, file);
		fclose(file);
	}

	if (file)
		fclose(file);
}


const char *get_filename(void) {
	return format_string("%s%s", get_timestamp(), LOGFILE);
}


const char *format_string(const char *fmt, ...) {
	int n;
	int size = 100;     /* Guess we need no more than 100 bytes */
	char *p, *np;
	va_list ap = (char *)NULL;

	if ((p = malloc(size)) == NULL)
		return NULL;

	while (1) {
		/* Try to print in the allocated space */
		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		/* Check error code */
		if (n < 0)
			continue;

		/* If that worked, return the string */
		if (n < size) {
			return p;
		}

		/* Else try again with more space */
		size = n + 1;       /* Precisely what is needed */

		if ((np = realloc(p, size)) == NULL) {
			free(p);
			continue;
		}
		else {
			p = np;
		}
	}
}


const char *get_timestamp() {
	//process ID
	unsigned long	processID = GetCurrentProcessId();

	//log file name
	time_t curTime;
	time(&curTime);
	struct tm tm1;
	localtime_s(&tm1, &curTime);
	return format_string(
		"%04d%02d%02d%02d%02d_%04d",
		tm1.tm_year + 1900,
		tm1.tm_mon + 1,
		tm1.tm_mday,
		tm1.tm_hour,
		tm1.tm_min,
		processID
	);
}


void LogInfo(const char *fmt, ...)
{
	int n;
	int size = 100;     /* Guess we need no more than 100 bytes */
	char *p, *np;
	va_list ap = (char *)NULL;

	if ((p = malloc(size)) == NULL)
		return;

	while (1) {
		/* Try to print in the allocated space */
		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		/* Check error code */
		if (n < 0)
			continue;

		/* If that worked, return the string */
		if (n < size) {
			Log(p);
			break;
		}

		/* Else try again with more space */
		size = n + 1;       /* Precisely what is needed */

		if ((np = realloc(p, size)) == NULL) {
			free(p);
			continue;
		}
		else {
			p = np;
		}
	}
	Log("\n");
}