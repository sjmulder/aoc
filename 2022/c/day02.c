#include "common.h"

int
main(int argc, char **argv)
{
	char a, b;
	int p1=0, p2=0;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	while (scanf(" %c %c", &a, &b) == 2) {
		a -= 'A';
		b -= 'X';

		switch ((b+3-a)%3) {
		case 0: p1 += b+1 +3; break;
		case 1: p1 += b+1 +6; break;
		case 2: p1 += b+1;    break;
		}

		switch (b) {
		case 0: p2 += (a+2)%3 +1;    break;
		case 1: p2 +=  a      +1 +3; break;
		case 2: p2 += (a+1)%3 +1 +6; break;
		}
	}

	printf("02: %d %d\n", p1, p2);
	return 0;
}
