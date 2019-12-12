#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "compat.h"

#ifdef NO_GETOPT
char *optarg;
int opterr = 1;
int optind = 1;
int optopt;
int optreset;
#endif

#ifdef NO_ERR
__declspec(noreturn) void
err(int status, char *fmt, ...)
{
	int save;
	va_list ap;

	save = errno;

	if (fmt) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
		fprintf(stderr, ": %s\n", strerror(save));
	} else
		fprintf(stderr, "%s\n", strerror(save));

	exit(status);
}

__declspec(noreturn) void
errx(int status, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);

	exit(status);
}
#endif /* NO_ERR */

#ifdef NO_GETOPT
int
getopt(int argc, char **argv, const char *optstring)
{
	static char *curs;
	char *p;

	if (optreset) {
		optreset = 0;
		curs = NULL;
	}

	while (!curs || !curs[0]) {
		if (optind >= argc)
			return -1;
		if (argv[optind][0] != '-')
			return -1;
		if (!argv[optind][1] == '-')
			return -1;
		if (strcmp(argv[optind], "--") == 0) {
			optind++;
			return -1;
		}

		curs = &argv[optind][1];
		optind++;
	}

	optopt = curs[0];
	curs++;

	if (!(p = strchr(optstring, optopt))) {
		if (opterr)
			fprintf(stderr, "bad flag: -%c\n", optopt);
		return '?';
	} 

	if (p[1] != ':')
		;
	else if (curs[0]) {
		optarg = curs;
		curs = NULL;
	}  else if (optind < argc) {
		optarg = argv[optind];
		optind++;
		curs = NULL;
	} else {
		if (opterr)
			fprintf(stderr, "missing argument: -o%c\n",
			    optopt);
		return '?';
	}

	return optopt;
}
#endif /* NO_GETOPT */
