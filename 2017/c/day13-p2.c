#define _CRT_SECURE_NO_WARNINGS
#define LEN(a) (sizeof(a)/sizeof(*a))

#include <stdio.h>

int
main()
{
	struct { int depth, range; } ls[64];
	int delay=0;
	size_t nls=0, i;

	while (1) {
		if (nls >= LEN(ls)) {
			fputs("too many ls\n", stderr);
			return 1;
		}

		if (scanf(" %d", &ls[nls].depth) != 1)
			break;
		if (scanf(" : %d", &ls[nls].range) != 1)
			break;
		nls++;
	}

	while (1) {
		for (i=0; i<nls; i++)
			if ((delay + ls[i].depth) % (ls[i].range*2-2) == 0)
				break;
		if (i == nls)
			break;
		delay++;
	}
	
	printf("%d\n", delay);
	return 0;
}
