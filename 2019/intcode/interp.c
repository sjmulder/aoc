#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#ifndef _WIN32
# include <unistd.h>
# include <err.h>
#endif
#include "compat.h"
#include "intcode.h"

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

static int64_t in_cb(void *);
static void out_cb(int64_t, void *);
static void usage(void);

static FILE *input;
static int prompt;

int
main(int argc, char **argv)
{
	static Icvm vm;
	int c;
	int verbosity = 0;
	FILE *f;

	while ((c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbosity++;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1 || argc > 2)
		usage();

	vm.in_cb = in_cb;
	vm.out_cb = out_cb;

	if (strcmp(argv[0], "-") == 0)
		ic_load(&vm, stdin);
	else {
		if (!(f = fopen(argv[0], "r")))
			err(1, "%s", argv[0]);
		ic_load(&vm, f);
	}

	if (argc < 2 || strcmp(argv[1], "-") == 0) {
		prompt = isatty(0);
		input = stdin;
	} else if (!(input = fopen(argv[1], "r")))
		err(1, "%s", argv[1]);

	while (!(vm.flags & IC_HALTED))
		ic_step(&vm, verbosity>1 ? stderr : NULL);

	if (verbosity)
		fprintf(stderr,
		    " steps: %d\n"
		    " words: %d\n"
		    " min value: %"PRId64"\n"
		    " max value: %"PRId64"\n",
		    vm.nsteps, vm.maxmem+1, vm.minval, vm.maxval);

	return 0;
}

static int64_t
in_cb(void *user)
{
	int64_t val;

	(void)user;

	if (prompt) {
		printf("> ");
		fflush(stdout);
	}
	if (fscanf(input, " %"PRId64, &val) != 1)
		errx(1, "unexpected input");

	return val;
}

static void
out_cb(int64_t val, void *user)
{
	(void)user;

	printf("%"PRId64"\n", val);
}

static void
usage(void)
{
	fprintf(stderr, "usage: intcode [-v] PROGRAM [INPUT]\n");
	exit(1);
}
