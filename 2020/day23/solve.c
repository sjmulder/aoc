#include <stdio.h>

#ifdef _MSC_VER
typedef __int64 i64;
# define PRi64 "%I64"
#else
typedef long long i64;
# define PRi64 "ll"
#endif


//#define INPUT		"389125467" /* sample */
#define INPUT		"326519478" /* input */
#define P1ROUNDS	100
#define P2ROUNDS	10000000
#define P2ENDNUM	1000000

static int succ[P2ENDNUM+1];

static void
run(int endnum, int nrounds)
{
	int i, first=0,min=0,max=0,cur=0,tmp,dest, gpstart,gpend;
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
		if ((dest = cur-1) < min)
			dest = max;
		while (dest == (tmp = succ[cur]) ||
		       dest == (tmp = succ[tmp]) ||
		       dest == (tmp = succ[tmp]))
			if ((dest = dest-1) < min)
				dest = max;

		gpstart = succ[cur];
		gpend   = succ[succ[gpstart]];
		succ[cur]   = succ[gpend];
		succ[gpend] = succ[dest];
		succ[dest]  = gpstart;
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
	printf(" %"PRi64"d\n", (i64)succ[1] * succ[succ[1]]);

	getchar();
	//return 0;
}
