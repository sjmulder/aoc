#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

typedef struct icvm {
	int mem[4096];
	int ic;
	int flags;
#define IC_HALTED 0x1
	FILE *input;
	FILE *output;
	FILE *prompt;
} Icvm;

typedef struct icop {
	int op;
#define IC_ADD 1
#define IC_MUL 2
#define IC_IN  3
#define IC_OUT 4
#define IC_JT  5
#define IC_JF  6
#define IC_LT  7
#define IC_EQ  8
#define IC_HLT 99
	int *args[3];
	int nin, nout;
	int nargs;
} Icop;

static void ic_step(Icvm *vm, FILE *log);

static void ic_load(Icvm *vm, FILE *f);
static void ic_decode(Icvm *vm, int pos, Icop *op);
static void ic_exec(Icvm *vm, Icop *op);
static void ic_log_pre(Icvm *vm, Icop *op, FILE *f);
static void ic_log_post(Icvm *vm, Icop *op, FILE *f);

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
ic_step(Icvm *vm, FILE *log)
{
	Icop op;
	int old_ic;

	if (vm->flags & IC_HALTED)
		return;
	if (vm->ic < 0 || vm->ic >= (int)LEN(vm->mem))
		errx(1, "ic out of bounds");

	old_ic = vm->ic;

	ic_decode(vm, vm->ic, &op);
	if (log)
		ic_log_pre(vm, &op, log);
	ic_exec(vm, &op);
	if (log)
		ic_log_post(vm, &op, log);

	if (vm->ic == old_ic)
		vm->ic += 1 + op.nargs;
}

static void
ic_load(Icvm *vm, FILE *f)
{
	int i = 0;

	while (fscanf(f, " %d ,", &vm->mem[i]) == 1)
		if (++i >= (int)LEN(vm->mem))
			errx(1, "program too large");
}

static void
ic_decode(Icvm *vm, int pos, Icop *op)
{
	int i, divisor, mode;

	if (pos < 0 || pos >= (int)LEN(vm->mem))
		errx(1, "pos %d: out of bounds", pos);

	op->op = vm->mem[pos] % 100;
	switch (op->op) {
	case IC_ADD: op->nin = 2; op->nout = 1; break;
	case IC_MUL: op->nin = 2; op->nout = 1; break;
	case IC_IN:  op->nin = 0; op->nout = 1; break;
	case IC_OUT: op->nin = 1; op->nout = 0; break;
	case IC_JT:  op->nin = 2; op->nout = 0; break;
	case IC_JF:  op->nin = 2; op->nout = 0; break;
	case IC_LT:  op->nin = 2; op->nout = 1; break;
	case IC_EQ:  op->nin = 2; op->nout = 1; break;
	case IC_HLT: op->nin = 0; op->nout = 0; break;
	default:
		errx(1, "pos %d: unknown op: %d", pos, op->op);
	}

	op->nargs = op->nin + op->nout;
	if (pos + op->nargs > (int)LEN(vm->mem))
		errx(1, "pos %d: args out of bounds: %d", pos,
		    op->nargs);

	divisor = 100;
	for (i = 0; i < op->nargs; i++) {
		mode = vm->mem[pos] / divisor % 10;
		switch (mode) {
		case 0: /* position */
			op->args[i] = &vm->mem[vm->mem[pos+i+1]];
			break;
		case 1: /* immediate */
			op->args[i] = &vm->mem[pos+i+1];
			break;
		default:
			errx(1, "pos %d: invalid mode %d for arg %d",
			    pos, mode, i);
		}
		if (op->args[i] < vm->mem ||
		    op->args[i] >= vm->mem + LEN(vm->mem))
			errx(1, "pos: %d: arg %d out of bound", pos, i);
		divisor *= 10;
	}

	if (vm->mem[pos] / divisor)
		errx(1, "pos %d: too many mode digits", pos);
}

static void
ic_exec(Icvm *vm, Icop *op)
{
	switch (op->op) {
	case IC_ADD:
		*op->args[2] = *op->args[0] + *op->args[1];
		break;
	case IC_MUL:
		*op->args[2] = *op->args[0] * *op->args[1];
		break;
	case IC_IN:
		if (vm->prompt) {
			fprintf(vm->prompt, "> ");
			fflush(vm->prompt);
		}
		if (!vm->input)
			errx(1, "IN: no input stream");
		if (fscanf(vm->input, " %d", op->args[0]) != 1)
			errx(1, "IN: expected input");
		break;
	case IC_OUT:
		if (!vm->output)
			errx(1, "OUT: no output stream");
		fprintf(vm->output, "%d\n", *op->args[0]);
		break;
	case IC_JT:
		if (*op->args[0])
			vm->ic = *op->args[1];
		break;
	case IC_JF:
		if (!*op->args[0])
			vm->ic = *op->args[1];
		break;
	case IC_LT:
		*op->args[2] = *op->args[0] < *op->args[1];
		break;
	case IC_EQ:
		*op->args[2] = *op->args[0] == *op->args[1];
		break;
	case IC_HLT:
		vm->flags |= IC_HALTED;
		break;
	default:
		errx(1, "unknown op: %d", op->op);
	}
}

static void
ic_log_pre(Icvm *vm, Icop *op, FILE *f)
{
	int i;

	fprintf(f, " %3d: ", vm->ic);

	switch (op->op) {
	case IC_ADD: fputs("add ", f); break;
	case IC_MUL: fputs("mul ", f); break;
	case IC_IN:  fputs("in  ", f); break;
	case IC_OUT: fputs("out ", f); break;
	case IC_JT:  fputs("jt  ", f); break;
	case IC_JF:  fputs("jf  ", f); break;
	case IC_LT:  fputs("jt  ", f); break;
	case IC_EQ:  fputs("eq  ", f); break;
	case IC_HLT: fputs("hlt ", f); break;
	default:
		errx(1, "unknown op: %d", op->op);
	}
	
	if (op->nargs > 0)
		fprintf(f, "[%3d]", (int)(op->args[0] - vm->mem));
	else
		fprintf(f, "     ");

	for (i = 1; i < op->nargs; i++)
		fprintf(f, ", [%3d]", (int)(op->args[i] - vm->mem));
	for (i = op->nargs; i < 3; i++)
		fprintf(f, "       ");

	if (op->nargs > 0)
		fprintf(f, " ; ");
	if (op->nin > 0)
		fprintf(f, "%7d", *op->args[0]);
	for (i = 1; i < op->nin; i++)
		fprintf(f, ",%7d", *op->args[i]);
	for (i = op->nin; i < 2; i++)
		fprintf(f, "       ");

	if (!op->nout)
		fputc('\n', f);
}

static void
ic_log_post(Icvm *vm, Icop *op, FILE *f)
{
	(void)vm;

	if (op->nout)
		fprintf(f, " -> %7d\n", *op->args[op->nin]);
}

static void
usage(void)
{
	fprintf(stderr, "usage: intcode [-v] PROGRAM [INPUT]\n");
	exit(1);
}
