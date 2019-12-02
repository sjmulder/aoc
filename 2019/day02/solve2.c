#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define OP_ADD 1
#define OP_MUL 2
#define OP_HALT 99

static size_t read(int *, size_t);
static int run(int *, size_t);

int
main()
{
	static int orig[4096];
	static int mem[4096];
	size_t len;
	int noun, verb;

	len = read(orig, LEN(orig));
	for (noun = 0; noun < 100; noun++)
		for (verb = 0; verb < 100; verb++) {
			memcpy(mem, orig, len * sizeof(int));
			mem[1] = noun;
			mem[2] = verb;
			if (run(mem, len) == 19690720) {
				printf("%d\n", 100 * noun + verb);
				return 0;
			}
		}

	printf("no solution\n");
	return 0;
}

static size_t
read(int *mem, size_t len)
{
	size_t i = 0;

	if (scanf("%d", &mem[i++]) != 1)
		errx(1, "no input");
	while (scanf(",%d", &mem[i]) == 1)
		if (++i >= len)
			errx(1, "out of mem");

	return i;
}

static int
run(int *mem, size_t len)
{
	int pc = 0;

	while (1) {
		if (pc >= len)
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
			return mem[0];
		default:
			errx(1, "invalid opcode %d at %d\n", mem[pc], pc);
		}
	}
}
