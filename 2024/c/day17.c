#include "common.h"

#define MEMZ 32
#define OUTZ 32
#define BUFZ 64

enum { ADV, BXL, BST, JNZ, BXC, OUT, BDV, CDV };

struct vm {
	int64_t mem[MEMZ]; int nm, pc;
	int64_t out[OUTZ]; int no;
	int64_t a,b,c;
} vm;

/* returns 0 if halted, 1 otherwise */
static int
step(void)
{
	int64_t op, ar, ac;	/* operator, lit. arg, combo arg */

	assert(vm.pc >= 0);
	assert(vm.pc+2 <= MEMZ);

	if (vm.pc >= vm.nm)
		return 0;

	op = vm.mem[vm.pc++];
	ar = vm.mem[vm.pc++];
	ac = ar==4 ? vm.a : ar==5 ? vm.b : ar==6 ? vm.c : ar;

	switch (op) {
	case ADV: vm.a = vm.a >> ac; break;
	case BDV: vm.b = vm.a >> ac; break;
	case CDV: vm.c = vm.a >> ac; break;
	case BXL: vm.b = vm.b ^ ar; break;
	case BXC: vm.b = vm.b ^ vm.c; break;
	case BST: vm.b = ac % 8; break;
	case JNZ: if (vm.a) vm.pc = ar; break;
	case OUT: assert(vm.no < OUTZ); vm.out[vm.no++] = ac%8; break;
	default: assert(!"invalid opcode"); return 0;
	}

	return 1;
}

static int64_t
recur_p2(int64_t a0, int pos)
{
	int64_t a,a1, i;

	/*
	 * The code in the input uses up to 7 low bits of the A register
	 * to produce a single number, then chops off the low 3 bits and
	 * continues.
	 *
	 * That means bits above the current triplet influence what
	 * number it generates, but bits below don't. To generate a
	 * desired sequence then, we append, not prepend,  candidate
	 * triplets.
	 *
	 * We may end up in a situation where, given the prefix found
	 * for the numbers so far, no triplet yields the desired next
	 * number. Then we have to backtrack and find another prefix,
	 * hence the recursion.
	 */

	if (pos >= vm.nm)
		return a0 >> 3;

	for (i=0; i<8; i++) {
		vm.a=a= a0+i;
		vm.b=vm.c=vm.pc=vm.no= 0;

		while (step() && !vm.no)
			;

		if (vm.no && vm.out[0] == vm.mem[vm.nm-pos-1])
			if ((a1 = recur_p2(a << 3, pos+1)))
				return a1;
	}

	return 0;
}

int
main(int argc, char **argv)
{
	char b[BUFZ], *tok, *rest;
	int i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	fgets(b, BUFZ, stdin); sscanf(b, "Register A: %lld", &vm.a);
	fgets(b, BUFZ, stdin); sscanf(b, "Register B: %lld", &vm.b);
	fgets(b, BUFZ, stdin); sscanf(b, "Register C: %lld", &vm.c);
	fgets(b, BUFZ, stdin);

	assert(vm.b == 0);	/* assumption for part 2 */
	assert(vm.c == 0);

	rest = fgets(b, sizeof(b), stdin);
	strsep(&rest, ":");

	while ((tok = strsep(&rest, ","))) {
		assert(vm.nm < MEMZ);
		vm.mem[vm.nm++] = atoll(tok);
	}

	while (step())
		;
	for (i=0; i<vm.no; i++)
		printf(i ? ",%lld" : "17: %lld", vm.out[i]);

	printf(" %lld\n", recur_p2(0, 0));
	return 0;
}
