#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../compat/builtin.h"

int
main(int argc, char **argv)
{
	FILE *f;
	int c=0, prev, p1=0, p2=0;
	unsigned mask=0, any=0, all=(1<<26)-1;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while ((prev = c) != EOF)
		if ((c = fgetc(f)) != '\n' && c != EOF)
			mask |= 1 << (c-'a');
		else if (prev == '\n') {
			p1 += popcount(any);
			p2 += popcount(all);
			any=0; all=(1<<26)-1;
		} else
			{ any |= mask; all &= mask; mask=0; }

	printf("%d %d\n", p1, p2);
	//getchar();
	return 0;
}
