#include <stdio.h>
#include <string.h>
#include <err.h>

#define MAXOPS 1024

enum {ONOP, OACC, OJMP};
struct op {int type; int arg; int hits;};

int
main()
{
	static struct op ops[MAXOPS];
	char name[4];
	int n, nops=0, acc=0,pc=0;

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

	while (1) {
		if (ops[pc].hits++)
			{ printf("%d\n", acc); return 0; }
		switch (ops[pc].type) {
			case OACC: acc += ops[pc].arg; break;
			case OJMP: pc += ops[pc].arg-1; break;
		}
		if (++pc<0 || pc >=nops)
			errx(1, "pc oob");
	}
}
