#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "compat.h"

#ifdef NO_STRSEP
char *
strsep(char **stringp, const char *delim)
{
	int i,j;
	char *s;

	if (!*stringp)
		return NULL;

	s = *stringp;

	for (i=0; s[i]; i++)
	for (j=0; delim[j]; j++)
		if (s[i] == delim[j]) {
			s[i] = '\0';
			*stringp = s+i+1;
			return s;
		}

	*stringp = NULL;
	return s;
}
#endif

#ifdef NO_ERR
noreturn void
err(int status, const char *fmt, ...)
{
	static char msg[256];
	va_list ap;
	int code;

	code = errno;

	if (fmt) {
		va_start(ap, fmt);
		vsnprintf(msg, sizeof(msg), fmt, ap);
		va_end(ap);

		fprintf(stderr, "%s: %s\n", msg, strerror(code));
	} else
		fprintf(stderr, "%s\n", strerror(code));

	exit(status);
}

noreturn void
errx(int status, const char *fmt, ...)
{
	va_list ap;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
	}

	exit(status);
}
#endif
