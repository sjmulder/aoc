#include "common.h"

/* returns 1 on sucess, 0 on overflow */
static int
concat(uint64_t a, uint64_t b, uint64_t *out)
{
	uint64_t mul;

	for (mul=1; mul<=b; mul*=10) ;

	return 
	    !mul_overflow( mul, a, out) &&
	    !add_overflow(*out, b, out);
}

static int
recur(uint64_t expect, uint64_t acc, uint64_t arr[], int n, int p2)
{
	uint64_t imm;

	return
	    n < 1 ? acc == expect :
	    acc >= expect ? 0 :
	    recur(expect, acc + arr[0], arr+1, n-1, p2) ||
	    recur(expect, acc * arr[0], arr+1, n-1, p2) ||
	    (p2 && concat(acc, arr[0], &imm)
	        && recur(expect, imm, arr+1, n-1, p2));
}

int
main(int argc, char **argv)
{
	char buf[128], *tok, *rest;
	uint64_t p1=0, p2=0, arr[32], expect;
	int n;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		assert(strchr(buf, '\n'));
		expect = atoll(strsep(&rest, ":"));

		for (n=0; (tok = strsep(&rest, " ")); n++) {
			assert(n < (int)LEN(arr));
			arr[n] = atoll(tok);
		}

		p1 += recur(expect, 0, arr, n, 0) * expect;
		p2 += recur(expect, 0, arr, n, 1) * expect;
	}

	printf("07: %"PRIu64" %"PRIu64"\n", p1, p2);
	return 0;
}
