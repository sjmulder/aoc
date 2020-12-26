#include <stdio.h>
#include "compat/stdint.h"
#include "compat/inttypes.h"

//#define INPUT		"389125467" /* sample */
#define INPUT		"326519478" /* input */
#define P1ROUNDS	100
#define P2ROUNDS	10000000
#define P2ENDNUM	1000000

static int succ[P2ENDNUM+1];

static void
run(int endnum, int nrounds)
{
	int i, first=0,min=0,max=0,cur=0,dest, gp1,gp2,gp3;
	const char *input = INPUT;

	while (*input)
		if ((cur = succ[cur] = *(input++)-'0') < min)
			min = cur;
		else if (!first)
			first = min = max = cur;
		else if (cur > max)
			max = cur;

	while (max < endnum)
		cur = succ[cur] = ++max;

	cur = succ[cur] = first;

	for (i=0; i < nrounds; i++) {
		gp1 = succ[cur]; gp2 = succ[gp1]; gp3 = succ[gp2];

		if ((dest = cur-1) < min)
			dest = max;
		while (dest == gp1 || dest == gp2 || dest == gp3)
			if ((dest = dest-1) < min)
				dest = max;

		succ[cur] = succ[gp3];
		succ[gp3] = succ[dest];
		succ[dest] = gp1;
		cur = succ[cur];
	}
}

int
main()
{
	int cur;

	run(0, P1ROUNDS);
	for (cur = succ[1]; cur != 1; cur = succ[cur])
		putchar('0'+cur);

	run(P2ENDNUM, P2ROUNDS);
	printf(" %" PRId64 "d\n", (int64_t)succ[1] * succ[succ[1]]);

	//getchar();
	return 0;
}
