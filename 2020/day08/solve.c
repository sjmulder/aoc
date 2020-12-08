#include <stdio.h>
#include <string.h>
#include <err.h>

#define MAXOPS 1024

enum {ONOP, OACC, OJMP};
struct op {int type, arg, hits;};
struct vm {int pc, acc;};

static void
run(struct vm *vmp, struct op *ops, int nops)
{
	static int hits[MAXOPS];
	struct vm vm = {};

	memset(hits, 0, sizeof(hits));

	while (1) {
		switch (ops[vm.pc].type) {
			case OACC: vm.acc += ops[vm.pc].arg; break;
			case OJMP: vm.pc += ops[vm.pc].arg-1; break;
		}
		if (++vm.pc<0 || vm.pc>=nops || hits[vm.pc]++)
			{ *vmp = vm; return; }
	}
}

int
main()
{
	static struct op ops[MAXOPS];
	struct vm vm = {};
	char name[4];
	int n,i, nops=0, new,old;

	while ((n = scanf("%3s %d", name, &ops[nops].arg)) > 0) {
		if (!strcmp(name, "nop") && n==2)
			ops[nops].type = ONOP;
		else if (!strcmp(name, "acc") && n==2)
			ops[nops].type = OACC;
		else if (!strcmp(name, "jmp") && n==2)
			ops[nops].type = OJMP;
		else
			errx(1, "bad op at line %d", nops+1);

		if (++nops >= MAXOPS)
			errx(1, "ops overflow");
	}

	run(&vm, ops, nops);
	printf("%d ", vm.acc);

	for (i=0; i<nops; i++) {
		old = new = ops[i].type;
		     if (old == ONOP) new = OJMP;
		else if (old == OJMP) new = ONOP;
		else continue;

		ops[i].type = new;
		memset(&vm, 0, sizeof(vm));
		run(&vm, ops, nops);
		ops[i].type = old;

		if (vm.pc == nops)
			{ printf("%d\n", vm.acc); return 0; }
	}
}
