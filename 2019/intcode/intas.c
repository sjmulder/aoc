#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "intcode.h"

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

typedef struct aslab {
	char name[32];
	int loc;
} Aslab;

typedef struct asarg {
	int val;
	int mode;
	int flags;
#define AS_UNRESOLVED 1
} Asarg;

typedef struct asop {
	int op;
	Asarg args[3];
	int nargs;
} Asop;

typedef struct as {
	Asop ops[4096];
	Aslab labs[512];
	size_t nops, nlabs;
	int ip;
} As;

static void as_read(As *as, FILE *f);
static void as_resolve(As *as);
static void as_write(As *as, FILE *f);

static void usage(void);

int
main(int argc, char **argv)
{
	static As as;
	int c;
	char *outpath = NULL;
	FILE *input, *output;

	while ((c = getopt(argc, argv, "o:")) != -1) {
		switch (c) {
		case 'o':
			outpath = optarg;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc > 1)
		usage();

	if (argc < 1 || strcmp("-", argv[0]) == 0)
		input = stdin;
	else if (!(input = fopen(argv[0], "r")))
		err(1, "%s", argv[0]);

	if (!outpath || strcmp("-", outpath) == 0)
		output = stdout;
	else if (!(output = fopen(outpath, "w")))
		err(1, "%s", outpath);

	as_read(&as, input);
	as_resolve(&as);
	as_write(&as, output);

	return 0;
}

static void
as_read(As *as, FILE *f)
{
	(void)as;
	(void)f;
	/* TODO */
}

static void
as_resolve(As *as)
{
	(void)as;
	/* TODO */
}

static void
as_write(As *as, FILE *f)
{
	(void)as;
	(void)f;
	/* TODO */

}

static
void usage(void)
{
	fprintf(stderr, "usage: intas [-o OUTPUT] [INPUT]\n");
	exit(1);
}
