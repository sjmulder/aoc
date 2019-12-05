#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "intcode.h"

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

static void usage(void);

int
main(int argc, char **argv)
{
	static Icvm vm;
	int c;
	int verbose = 0;
	FILE *f;

	while ((c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
		case 'v':
			verbose = 1;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1 || argc > 2)
		usage();

	if (strcmp(argv[0], "-") == 0)
		ic_load(&vm, stdin);
	else {
		if (!(f = fopen(argv[0], "r")))
			err(1, "%s", argv[0]);
		ic_load(&vm, f);
	}

	if (argc < 2 || strcmp(argv[1], "-") == 0) {
		vm.input = stdin;
		if (isatty(0))
			vm.prompt = stdout;
	} else if (!(vm.input = fopen(argv[1], "r")))
		err(1, "%s", argv[1]);

	vm.output = stdout;

	while (!(vm.flags & IC_HALTED))
		ic_step(&vm, verbose ? stderr : NULL);

	return 0;
}

static void
usage(void)
{
	fprintf(stderr, "usage: intcode [-v] PROGRAM [INPUT]\n");
	exit(1);
}
