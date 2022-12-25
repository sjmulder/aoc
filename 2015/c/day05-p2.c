#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

int
main()
{
	static char b[1024*1024];
	int p2=0, pair=0, rpt=0;
	size_t sz, st, i;

	sz = fread(b, 1, sizeof(b), stdin);
	st = 0;

	for (i=0; i<sz; i++) {
		if (b[i] == '\n') {
			p2 += pair && rpt;
			pair = rpt = 0;
			st = i+1;
		} else {
			pair = pair || (i > st+2 &&
			    !!memmem(&b[st], i-st-1, &b[i-1], 2));
			rpt = rpt || (i > st+1 && b[i-2] == b[i]);
		}
	}

	printf("%d\n", p2);
	return 0;
}
