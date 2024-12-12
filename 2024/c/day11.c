#include "common.h"

/* returns 1 and splits x if even-digited, 0 otherwise */
static int
split(uint64_t x, uint64_t *a, uint64_t *b)
{
	uint64_t p;
	int n, i;

	if (!x) return 0;
	for (n=0, p=1; p<=x;  n++, p*=10) ; if (n%2) return 0;
	for (i=0, p=1; i<n/2; i++, p*=10) ;
	*a = x/p;
	*b = x%p; return 1;
}

/*
 * recur() is memoized in mem[]. Testing found the size MEMZ to be optimal:
 * lowering siginificantly reduced hits, but raising tenfold didn't add a
 * single hit.
 */

#define MEMZ (1024*1024)
static uint64_t mem[MEMZ][76];

static uint64_t
recur(uint64_t v, int n)
{
	uint64_t a,b;

	if (n<1 ) return 1;
	if (v==0) return recur(1, n-1);
	if (v<10) return recur(v*2024, n-1);
	if (v<MEMZ && mem[v][n]) return mem[v][n];
	if (!split(v, &a, &b))   return recur(v*2024, n-1);

	return v<MEMZ ? mem[v][n] =
	    recur(a, n-1) + recur(b, n-1) :
	    recur(a, n-1) + recur(b, n-1);
}

int
main(int argc, char **argv)
{
	uint64_t p1=0,p2=0, val;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (scanf(" %"SCNu64, &val) == 1) {
		p1 += recur(val, 25);
		p2 += recur(val, 75);
	}

	printf("11: %"PRId64" %"PRId64"\n", p1, p2);
	return 0;
}
