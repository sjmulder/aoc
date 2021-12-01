#include <stdio.h>

int
main()
{
	int cur, prev, ninc=0;

	prev = scanf(" %d", &prev);

	while (scanf(" %d", &cur) == 1) {
		ninc += cur > prev;
		prev = cur;
	}

	printf("%d\n", ninc);
}
