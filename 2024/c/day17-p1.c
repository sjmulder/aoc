#include "common.h"

#define MEMZ 32
#define OUTZ 64

enum {
	OP_ADV,
	OP_BXL,
	OP_BST,
	OP_JNZ,
	OP_BXC,
	OP_OUT,
	OP_BDV,
	OP_CDV
};

const char * const ac_names[] = {"0","1","2","3","a","b","c","7"};

struct vm {
	int mem[MEMZ];
	char out[OUTZ];
	int nm;		/* mem length */
	int pc;		/* instruction pointer */
	int a,b,c;	/* registers */
} vm;

static void
print_state(void)
{
	printf("PC=%04d A=%8d B=%8d C=%8d",
	    vm.pc, vm.a, vm.b, vm.c);
}

static void
print_ins(int op, int ar)
{
	assert(ar >= 0);
	assert(ar < (int)LEN(ac_names));

	switch (op) {
	case OP_ADV: printf("adv %s", ac_names[ar]); break;
	case OP_BDV: printf("bdv %s", ac_names[ar]); break;
	case OP_CDV: printf("cdv %s", ac_names[ar]); break;
	case OP_BXL: printf("bxl %d", ar); break;
	case OP_BXC: printf("bxc"); break;
	case OP_BST: printf("bst %s", ac_names[ar]); break;
	case OP_JNZ: printf("jnz %d", ar); break;
	case OP_OUT: printf("out %s", ac_names[ar]); break;
	default: printf("<bad op>"); break;
	}
}

static void
print_disas(int *mem, int off, int len)
{
	int i;

	for (i=off; i+1 < off+len; i+=2) {
		printf("%04d | %d %d | ", i, mem[i], mem[i+1]);
		print_ins(mem[i], mem[i+1]);
		printf("\n");
	}
}

/* returns 0 if halted, 1 otherwise */
static int
step(void)
{
	int op;		/* operator */
	int ar;		/* literal argument */
	int ac;		/* combo argument */
	int outlen;

	assert(vm.pc >= 0);
	assert(vm.pc+2 <= MEMZ);

	if (vm.pc >= vm.nm) {
		print_state();
		printf("\n");
		return 0;
	}

	print_state();
	printf(" | ");

	op = vm.mem[vm.pc++];
	ar = vm.mem[vm.pc++];

	printf("%d %d | ", op, ar);
	print_ins(op, ar);
	printf("\n");

	switch (ar) {
	case 4: ac = vm.a; break;
	case 5: ac = vm.b; break;
	case 6: ac = vm.c; break;
	default: ac = ar;
	}

	switch (op) {
	case OP_ADV: vm.a = vm.a >> ac; break;
	case OP_BDV: vm.b = vm.a >> ac; break;
	case OP_CDV: vm.c = vm.a >> ac; break;
	case OP_BXL: vm.b = vm.b ^ ar; break;
	case OP_BXC: vm.b = vm.b ^ vm.c; break;
	case OP_BST: vm.b = ac % 8; break;
	case OP_JNZ: if (vm.a) vm.pc = ar; break;
	case OP_OUT:
		if (vm.out[0]) {
			outlen = strlen(vm.out);
			assert(outlen < OUTZ-1);
			snprintf(vm.out+outlen, OUTZ-outlen,
			    ",%d", ac%8);
		} else
			snprintf(vm.out, OUTZ, "%d", ac%8);
		break;
	default: assert(!"invalid opcode"); return 0;
	}

	return 1;
}

int
main(int argc, char **argv)
{
	char b[64], *tok, *rest;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	fgets(b, sizeof(b), stdin); sscanf(b, "Register A: %d", &vm.a);
	fgets(b, sizeof(b), stdin); sscanf(b, "Register B: %d", &vm.b);
	fgets(b, sizeof(b), stdin); sscanf(b, "Register C: %d", &vm.c);

	fgets(b, sizeof(b), stdin);

	rest = fgets(b, sizeof(b), stdin);
	strsep(&rest, ":");

	while ((tok = strsep(&rest, ","))) {
		assert(vm.nm < MEMZ);
		vm.mem[vm.nm++] = atoi(tok);
	}

	print_disas(vm.mem, 0, vm.nm);

	while (step())
		;

	printf("17: %s _\n", vm.out);
	return 0;
}
