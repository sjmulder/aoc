#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#ifndef _WIN32
# include <err.h>
#endif
#include "intcode.h"
#include "compat.h"

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
	uint64_t i = 0;

	while (fscanf(f, " %"PRId64" ,", &vm->mem[i]) == 1)
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
	case IC_ADD:   op->nin = 2; op->nout = 1; break;
	case IC_MUL:   op->nin = 2; op->nout = 1; break;
	case IC_IN:    op->nin = 0; op->nout = 1; break;
	case IC_OUT:   op->nin = 1; op->nout = 0; break;
	case IC_JNZ:   op->nin = 2; op->nout = 0; break;
	case IC_JZ:    op->nin = 2; op->nout = 0; break;
	case IC_LT:    op->nin = 2; op->nout = 1; break;
	case IC_EQ:    op->nin = 2; op->nout = 1; break;
	case IC_ADDBP: op->nin = 1; op->nout = 0; break;
	case IC_HLT:   op->nin = 0; op->nout = 0; break;
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
		op->args[i].val = vm->mem[pos+1+i];
		divisor *= 10;
	}

	if (vm->mem[pos] / divisor)
		errx(1, "pos %d: too many mode digits", pos);
}

int64_t *
ic_deref(Icvm *vm, Icarg *arg, int pos)
{
	int64_t *p;

	switch (arg->mode) {
	case IC_INDIRECT:
		p = &vm->mem[arg->val];
		if (p < vm->mem || p >= vm->mem + LEN(vm->mem))
			errx(1, "pos %d: arg out of bounds: %"PRId64,
			    pos, arg->val);
		break;
	case IC_IMMEDIATE:
		p = &arg->val;
		break;
	case IC_RELATIVE:
		p = &vm->mem[vm->bp + arg->val];
		if (p < vm->mem || p >= vm->mem + LEN(vm->mem))
			errx(1, "pos %d: arg out of  bounds: "
			    "bp+%"PRId64" (%"PRId64")", pos,
			    arg->val, vm->bp + arg->val);
		break;
	default:
		errx(1, "pos %d: invalid mode %d for arg", pos,
		    arg->mode);
	}

	return p;
}

void
ic_exec(Icvm *vm, Icop *op)
{
	int i;
	int64_t *argps[LEN(op->args)];

	for (i = 0; i < op->nargs; i++)
		argps[i] = ic_deref(vm, &op->args[i], vm->ic+1+i);

	switch (op->op) {
	case IC_ADD:
		*argps[2] = *argps[0] + *argps[1];
		break;
	case IC_MUL:
		*argps[2] = *argps[0] * *argps[1];
		break;
	case IC_IN:
		if (!vm->in_cb)
			errx(1, "IN: no input callback");
		*argps[0] = vm->in_cb(vm->user);
		break;
	case IC_OUT:
		if (!vm->out_cb)
			errx(1, "OUT: no output callback");
		vm->out_cb(*argps[0], vm->user);
		break;
	case IC_JNZ:
		if (*argps[0])
			vm->ic = *argps[1];
		break;
	case IC_JZ:
		if (!*argps[0])
			vm->ic = *argps[1];
		break;
	case IC_LT:
		*argps[2] = *argps[0] < *argps[1];
		break;
	case IC_EQ:
		*argps[2] = *argps[0] == *argps[1];
		break;
	case IC_ADDBP:
		vm->bp += *argps[0];
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
	int64_t *argps[LEN(op->args)];

	for (i = 0; i < op->nargs; i++)
		argps[i] = ic_deref(vm, &op->args[i], vm->ic+1+i);

	fprintf(f, " %3d: %05"PRId64" ", vm->ic, vm->mem[vm->ic]);

	switch (op->op) {
	case IC_ADD:   fputs("add   ", f); break;
	case IC_MUL:   fputs("mul   ", f); break;
	case IC_IN:    fputs("in    ", f); break;
	case IC_OUT:   fputs("out   ", f); break;
	case IC_JNZ:   fputs("jnz   ", f); break;
	case IC_JZ:    fputs("jz    ", f); break;
	case IC_LT:    fputs("lt    ", f); break;
	case IC_EQ:    fputs("eq    ", f); break;
	case IC_ADDBP: fputs("addbp ", f); break;
	case IC_HLT:   fputs("hlt   ", f); break;
	default:
		errx(1, "unknown op: %d", op->op);
	}
	
	if (op->nargs == 0)
		fprintf(f, "       ");
	for (i = 0; i < op->nargs; i++) {
		if (i)
			fputc(',', f);
		switch (op->args[i].mode) {
		case IC_INDIRECT:
			fprintf(f, "  [%3"PRId64"]", op->args[i].val);
			break;
		case IC_IMMEDIATE:
			fprintf(f, "%7"PRId64"", op->args[i].val);
			break;
		case IC_RELATIVE:
			fprintf(f, "[bp%+3"PRId64"]", op->args[i].val);
			break;
		default:
			errx(1, "unknown mode: %d", op->args[i].mode);
		}
	}
	for (i = op->nargs; i < 3; i++)
		fprintf(f, "        ");

	if (op->nargs > 0)
		fprintf(f, " ;");
	if (op->nin > 0)
		fprintf(f, " %7"PRId64, *argps[0]);
	for (i = 1; i < op->nin; i++)
		fprintf(f, ",%7"PRId64, *argps[i]);
	for (i = op->nin; i < 2; i++)
		fprintf(f, "        ");

	if (!op->nout)
		fputc('\n', f);
}

void
ic_log_post(Icvm *vm, Icop *op, FILE *f)
{
	int64_t *argp;

	if (op->nout) {
		argp = ic_deref(vm, &op->args[op->nin],
		    vm->ic+1+op->nout);
		fprintf(f, " -> %7"PRId64"\n", *argp);
	}
}
