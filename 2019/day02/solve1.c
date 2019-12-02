#include <stdio.h>
#include <err.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define OP_ADD 1
#define OP_MUL 2
#define OP_HALT 99

static int mem[4096];

int
main()
{
	int pc = 0;

	if (scanf("%d", &mem[pc++]) != 1)
		errx(1, "no input");
	while (scanf(",%d", &mem[pc]) == 1)
		if (++pc >= LEN(mem))
			errx(1, "out of mem");

	mem[1] = 12;
	mem[2] = 2;

	pc = 0;
	while (1) {
		if (pc >= LEN(mem))
			errx(1, "pc out of bounds");
		switch (mem[pc]) {
		case OP_ADD:
			mem[mem[pc+3]] = mem[mem[pc+1]] + mem[mem[pc+2]];
			pc += 4;
			break;
		case OP_MUL:
			mem[mem[pc+3]] = mem[mem[pc+1]] * mem[mem[pc+2]];
			pc += 4;
			break;
		case OP_HALT:
			printf("%d\n", mem[0]);
			return 0;
		default:
			errx(1, "invalid opcode %d at %d\n", mem[pc], pc);
		}
	}
}
