#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
	char ref[32];
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
	int nops, nlabs;
	char *curs;
	int loc;
} As;

static void as_read(As *as, FILE *f);

static Asop *as_rdop(As *as);
static Asarg *as_rdarg(As *as, Asop *op);
static Aslab *as_rdlabel(As *as);
static void as_rdspace(As *as);
static int as_rdident(As *as, char *s, size_t sz);
static int as_rdnum(As *as, int *n);

static void as_resolve(As *as);
static void as_write(As *as, FILE *f);
static void as_dump(As *as, FILE *f);

static void usage(void);

int
main(int argc, char **argv)
{
	static As as;
	int c, verbose = 0;
	char *outpath = NULL;
	FILE *input, *output;

	while ((c = getopt(argc, argv, "vo:")) != -1) {
		switch (c) {
		case 'v':
			verbose = 1;
			break;
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
	if (verbose)
		as_dump(&as, stderr);
	as_resolve(&as);
	as_write(&as, output);

	return 0;
}

static void
as_read(As *as, FILE *f)
{
	static char ln[512];
	int lineno = 0;
	char *c;
	
	while (fgets(ln, LEN(ln), f)) {
		if ((c = strchr(ln, '#')))
			*c = '\0';
		else if ((c = strchr(ln, '\n')))
			*c = '\0';

		lineno++;
		as->curs = ln;

		as_rdspace(as);
		while (*as->curs) {
			while (as_rdlabel(as))
				as_rdspace(as);
			if (as_rdop(as)) {
				as_rdspace(as);
				if (*as->curs == ';')
					as->curs++;
			} else if (*as->curs) {
				fprintf(stderr, "input:%d: expected "
				    "label or statement at col %d:\n"
				    "  %s\n",
				    lineno, (int)(as->curs-ln+1), ln);
				exit(1);
			}
			as_rdspace(as);
		}
	}

	if (ferror(f))
		err(1, "input");
}

static Asop *
as_rdop(As *as)
{
	Asop *op;
	Asarg *arg;
	char *curs_save, name[32];
	int loc_save, argfac = 100;
	
	curs_save = as->curs;
	loc_save = as->loc;

	if (as->nops >= (int)LEN(as->ops))
		errx(1, "ops overflow");
	
	op = &as->ops[as->nops];
	memset(op, 0, sizeof(*op));

	if (!as_rdident(as, name, LEN(name)))
		goto nope;
	
	if      (strcmp(name, "add") == 0) op->op = IC_ADD;
	else if (strcmp(name, "mul") == 0) op->op = IC_MUL;
	else if (strcmp(name, "in" ) == 0) op->op = IC_IN;
	else if (strcmp(name, "out") == 0) op->op = IC_OUT;
	else if (strcmp(name, "jt" ) == 0) op->op = IC_JT;
	else if (strcmp(name, "jf" ) == 0) op->op = IC_JF;
	else if (strcmp(name, "lt" ) == 0) op->op = IC_LT;
	else if (strcmp(name, "eq" ) == 0) op->op = IC_EQ;
	else if (strcmp(name, "hlt") == 0) op->op = IC_HLT;
	else
		goto nope;
	
	as->loc++;

	as_rdspace(as);
	while (1) {
		while (as_rdlabel(as))
			as_rdspace(as);
		if (!(arg = as_rdarg(as, op)))
			break;
		
		op->op += argfac * arg->mode;
		argfac *= 10;
		
		as_rdspace(as);
		if (*as->curs != ',')
			break;
		as->curs++;
		as_rdspace(as);
	}

	as->nops++;
	return op;

nope:
	as->curs = curs_save;
	as->loc = loc_save;
	return NULL;
}

static Asarg *
as_rdarg(As *as, Asop *op)
{
	Asarg *arg;
	char *curs_save;

	curs_save = as->curs;

	if (op->nargs >= (int)LEN(op->args))
		errx(1, "args overflow");
	
	arg = &op->args[op->nargs];
	memset(arg, 0, sizeof(*arg));
	
	as_rdspace(as);
	if (*as->curs == '[') {
		as->curs++;
		arg->mode = IC_INDIRECT;
		as_rdspace(as);
	} else
		arg->mode = IC_IMMEDIATE;

	if (as_rdnum(as, &arg->val))
		;
	else if (as_rdident(as, arg->ref, (int)LEN(arg->ref)))
		arg->flags |= AS_UNRESOLVED;
	else
		goto nope;
	
	if (arg->mode == IC_INDIRECT) {
		as_rdspace(as);
		if (*as->curs != ']')
			goto nope;
		as->curs++;
	}

	op->nargs++;
	as->loc++;
	return arg;

nope:
	as->curs = curs_save;
	return NULL;
}

static Aslab *
as_rdlabel(As *as)
{
	Aslab *lab;
	char *curs_save;

	curs_save = as->curs;

	if (as->nlabs >= (int)LEN(as->labs))
		errx(1, "labs overflow");

	lab = &as->labs[as->nlabs];
	memset(lab, 0, sizeof(*lab));
	lab->loc = as->loc;

	if (!as_rdident(as, lab->name, (int)LEN(lab->name)))
		goto nope;
	as_rdspace(as);
	if (*as->curs != ':')
		goto nope;
	as->curs++;
	
	as->nlabs++;
	return lab;

nope:
	as->curs = curs_save;
	return NULL;
}

static void
as_rdspace(As *as)
{
	while (*as->curs && isspace(*as->curs))
		as->curs++;
}

static int
as_rdident(As *as, char *s, size_t sz)
{
	char *curs;
	size_t len = 0;

	curs = as->curs;

	if (isalpha(*curs) || *curs == '_') {
		if (len+1 >= sz)
			errx(1, "ident overflow");
		s[len++] = *(curs++);
	} else
		return 0;
	
	while (isalnum(*curs) || *curs == '_') {
		if (len+1 >= sz)
			errx(1, "ident overflow");
		s[len++] = *(curs++);
	}

	s[len] = '\0';
	as->curs = curs;
	return 1;
}

static int
as_rdnum(As *as, int *n)
{
	char *end;

	*n = (int)strtol(as->curs, &end, 10);
	if (as->curs == end)
		return 0;
	
	as->curs = end;
	return 1;
}

static void
as_resolve(As *as)
{
	int i, j, k;
	Asarg *arg;

	for (i = 0; i < as->nops; i++)
	for (j = 0; j < as->ops[i].nargs; j++) {
		arg = &as->ops[i].args[j];
		if (!(arg->flags & AS_UNRESOLVED))
			continue;
		for (k = 0; k < as->nlabs; k++)
			if (!strcmp(as->labs[k].name, arg->ref))
				break;
		if (k == as->nlabs) {
			fprintf(stderr, "unresolved reference: %s\n",
			    arg->ref);
			exit(1);
		}
		arg->flags &= ~AS_UNRESOLVED;
		arg->val = as->labs[k].loc;
	}
}

static void
as_write(As *as, FILE *f)
{
	int i, j;
	Asop *op;

	for (i = 0; i < as->nops; i++) {
		op = &as->ops[i];
		fprintf(f, i ? ",%d" : "%d", op->op);
		for (j = 0; j < op->nargs; j++)
			fprintf(f, ",%d", op->args[j].val);
	}

	printf("\n");
}

static void
as_dump(As *as, FILE *f)
{
	int i, j;
	Asop *op;
	Asarg *arg;

	fprintf(f, "labels:\n");
	for (i = 0; i < as->nlabs; i++) 
		fprintf(f, " %d %s\n", as->labs[i].loc,
		    as->labs[i].name);
	
	fprintf(f, "ops:\n");
	for (i = 0; i < as->nops; i++) {
		op = &as->ops[i];
		fprintf(f, " %d", op->op);

		for (j = 0; j < op->nargs; j++) {
			arg = &op->args[j];
			fprintf(f, ", ");
			if (arg->mode == IC_INDIRECT)
				fprintf(f, "[");
			if (arg->flags & AS_UNRESOLVED)
				fprintf(f, "%s", arg->ref);
			else
				fprintf(f, "%d", arg->val);
			if (arg->mode == IC_INDIRECT)
				fprintf(f, "]");
		}

		fprintf(f, "\n");
	}
}

static void
usage(void)
{
	fprintf(stderr, "usage: intas [-v] [-o OUTPUT] [INPUT]\n");
	exit(1);
}
