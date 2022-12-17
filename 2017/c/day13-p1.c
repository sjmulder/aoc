#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int
main()
{
	int depth, range;
	int severity = 0;

	while (1) {
		if (scanf(" %d", &depth) != 1)
			break;
		if (scanf(" : %d", &range) != 1)
			break;
		if (depth % (range*2-2) == 0)
			severity += depth * range;
	}
	
	printf("%d\n", severity);
	return 0;
}
