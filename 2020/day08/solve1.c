#include <stdio.h>
#include <string.h>
#include <err.h>

enum {ONOP, OACC, OJMP};
struct op {int type; int arg; int hits;};

int main() {
	static struct op ops[1024];
	char name[4];
	int nops=0, acc=0,pc=0;

	while (scanf("%3s %d", name, &ops[nops].arg) == 2) {
		     if (!strcmp(name, "nop")) ops[nops].type = ONOP;
		else if (!strcmp(name, "acc")) ops[nops].type = OACC;
		else if (!strcmp(name, "jmp")) ops[nops].type = OJMP;
		else errx(1, "bad op at line %d", nops+1);

		if (++nops >= 1024)
			errx(1, "ops overflow");
	}

	do switch (ops[pc].type) {
		case OACC: acc += ops[pc].arg;   break;
		case OJMP: pc  += ops[pc].arg-1; break;
	} while (++pc>=0 && pc<nops && !ops[pc].hits++);

	printf("%d\n", acc);
}
