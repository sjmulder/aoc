#include <stdio.h>
#include <err.h>
#include "intcode.h"

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

void
ic_run(Icvm *vm, FILE *log)
{
	while (!(vm->flags & IC_HALTED))
		ic_step(vm, log);
}

void
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

void
ic_load(Icvm *vm, FILE *f)
{
	int i = 0;

	while (fscanf(f, " %d ,", &vm->mem[i]) == 1)
		if (++i >= (int)LEN(vm->mem))
			errx(1, "program too large");
}

void
ic_decode(Icvm *vm, int pos, Icop *op)
{
	int i, divisor;

	if (pos < 0 || pos >= (int)LEN(vm->mem))
		errx(1, "pos %d: out of bounds", pos);

	op->op = vm->mem[pos] % 100;
	switch (op->op) {
	case IC_ADD: op->nin = 2; op->nout = 1; break;
	case IC_MUL: op->nin = 2; op->nout = 1; break;
	case IC_IN:  op->nin = 0; op->nout = 1; break;
	case IC_OUT: op->nin = 1; op->nout = 0; break;
	case IC_JNZ: op->nin = 2; op->nout = 0; break;
	case IC_JZ:  op->nin = 2; op->nout = 0; break;
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
		op->args[i].mode = vm->mem[pos] / divisor % 10;
		switch (op->args[i].mode) {
		case IC_INDIRECT:
			op->args[i].p = &vm->mem[vm->mem[pos+i+1]];
			break;
		case IC_IMMEDIATE:
			op->args[i].p = &vm->mem[pos+i+1];
			break;
		default:
			errx(1, "pos %d: invalid mode %d for arg %d",
			    pos, op->args[i].mode, i);
		}
		if (op->args[i].p < vm->mem ||
		    op->args[i].p >= vm->mem + LEN(vm->mem))
			errx(1, "pos: %d: arg %d out of bound", pos, i);
		divisor *= 10;
	}

	if (vm->mem[pos] / divisor)
		errx(1, "pos %d: too many mode digits", pos);
}

void
ic_exec(Icvm *vm, Icop *op)
{
	switch (op->op) {
	case IC_ADD:
		*op->args[2].p = *op->args[0].p + *op->args[1].p;
		break;
	case IC_MUL:
		*op->args[2].p = *op->args[0].p * *op->args[1].p;
		break;
	case IC_IN:
		if (!vm->in_cb)
			errx(1, "IN: no input callback");
		*op->args[0].p = vm->in_cb(vm->user);
		break;
	case IC_OUT:
		if (!vm->out_cb)
			errx(1, "OUT: no output callback");
		vm->out_cb(*op->args[0].p, vm->user);
		break;
	case IC_JNZ:
		if (*op->args[0].p)
			vm->ic = *op->args[1].p;
		break;
	case IC_JZ:
		if (!*op->args[0].p)
			vm->ic = *op->args[1].p;
		break;
	case IC_LT:
		*op->args[2].p = *op->args[0].p < *op->args[1].p;
		break;
	case IC_EQ:
		*op->args[2].p = *op->args[0].p == *op->args[1].p;
		break;
	case IC_HLT:
		vm->flags |= IC_HALTED;
		break;
	default:
		errx(1, "unknown op: %d", op->op);
	}
}

void
ic_log_pre(Icvm *vm, Icop *op, FILE *f)
{
	int i;

	fprintf(f, " %3d: %05d ", vm->ic, vm->mem[vm->ic]);

	switch (op->op) {
	case IC_ADD: fputs("add ", f); break;
	case IC_MUL: fputs("mul ", f); break;
	case IC_IN:  fputs("in  ", f); break;
	case IC_OUT: fputs("out ", f); break;
	case IC_JNZ: fputs("jnz ", f); break;
	case IC_JZ:  fputs("jz  ", f); break;
	case IC_LT:  fputs("lt  ", f); break;
	case IC_EQ:  fputs("eq  ", f); break;
	case IC_HLT: fputs("hlt ", f); break;
	default:
		errx(1, "unknown op: %d", op->op);
	}
	
	if (op->nargs == 0)
		fprintf(f, "       ");
	else if (op->args[0].mode == IC_IMMEDIATE)
		fprintf(f, "%7d", *op->args[0].p);
	else
		fprintf(f, "  [%3d]", (int)(op->args[0].p - vm->mem));

	for (i = 1; i < op->nargs; i++) {
		if (op->args[i].mode == IC_IMMEDIATE)
			fprintf(f, ",%7d", *op->args[i].p);
		else
			fprintf(f, ",  [%3d]",
			    (int)(op->args[i].p - vm->mem));
	}
	for (i = op->nargs; i < 3; i++)
		fprintf(f, "        ");

	if (op->nargs > 0)
		fprintf(f, " ;");
	if (op->nin > 0)
		fprintf(f, " %7d", *op->args[0].p);
	for (i = 1; i < op->nin; i++)
		fprintf(f, ",%7d", *op->args[i].p);
	for (i = op->nin; i < 2; i++)
		fprintf(f, "        ");

	if (!op->nout)
		fputc('\n', f);
}

void
ic_log_post(Icvm *vm, Icop *op, FILE *f)
{
	(void)vm;

	if (op->nout)
		fprintf(f, " -> %7d\n", *op->args[op->nin].p);
}
