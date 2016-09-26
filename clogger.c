#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "clogger.h"


int LogCreated = FALSE;


void Log(char *message)
{
	FILE *file;

	if (!LogCreated) {
		file = fopen(LOGFILE, "w");
		LogCreated = TRUE;
	} else {
		file = fopen(LOGFILE, "a");
	}

	if (file == NULL) {
		if (LogCreated)
			LogCreated = FALSE;
		return;
	} else {
		fputs(message, file);
		fclose(file);
	}

	if (file)
		fclose(file);
}


void LogErr(const char *fmt, ...)
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