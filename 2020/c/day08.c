#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAXOPS 1024

enum {ONOP, OACC, OJMP};
static struct {int type, arg, hits;} ops[MAXOPS];
static struct {int pc, acc;} vm;
static int nops;

static void
run(void)
{
	int hits[MAXOPS];

	memset(hits, 0, sizeof(hits));

	do switch (ops[vm.pc].type) {
		case OACC: vm.acc += ops[vm.pc].arg;   break;
		case OJMP: vm.pc  += ops[vm.pc].arg-1; break;
	} while (++vm.pc>=0 && vm.pc<nops && !hits[vm.pc]++);
}

int
main(int argc, char **argv)
{
	FILE *f;
	char s[4];
	int i, new,old;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while ((fscanf(f, "%3s %d", s, &ops[nops].arg)) == 2) {
		     if (!strcmp(s, "nop")) ops[nops].type = ONOP;
		else if (!strcmp(s, "acc")) ops[nops].type = OACC;
		else if (!strcmp(s, "jmp")) ops[nops].type = OJMP;
		else assert(0 && "bad op");

		nops++;
		assert(nops < MAXOPS);
	}

	run();
	printf("%d ", vm.acc);

	for (i=0; i<nops && vm.pc != nops; i++) {
		old = new = ops[i].type;
		     if (old == ONOP) new = OJMP;
		else if (old == OJMP) new = ONOP;
		else continue;

		memset(&vm, 0, sizeof(vm));
		ops[i].type = new; run();
		ops[i].type = old;
	}

	printf("%d\n", vm.acc);
	//getchar();
	return 0;
}
