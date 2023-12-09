#include "common.h"

int
main(int argc, char **argv)
{
	static char line[128];
	static int a[24][24];
	size_t n,d,i;
	char *tok, *rest;
	int p1=0,p2=0, nz, acc1,acc2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while ((rest = fgets(line, sizeof(line), stdin))) {
		for (n=0; (tok = strsep(&rest, " ")); n++) {
			assert (n < LEN(*a));
			a[0][n] = atoi(tok);
		}

		/* geenrate rows until all 0, 'd' is depth */
		for (d=1, nz=1; nz && d<n; d++)
		for (i=0, nz=0; i<n-d; i++)
			nz |= (a[d][i] = a[d-1][i+1] - a[d-1][i]);

		/* extrapolate forward and backwards */
		for (i=0, acc1=acc2=0; i<d; i++) {
			acc1 += a[d-i-1][n-d+i];
			acc2  = a[d-i-1][0] - acc2;
		}

		p1 += acc1;
		p2 += acc2;
	}

	printf("09: %d %d\n", p1, p2);
	return 0;
}
