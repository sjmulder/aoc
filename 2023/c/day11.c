#include "common.h"

#define MAXPT	512
#define GSZ	148
#define MUL	(1000000-1)

int
main(int argc, char **argv)
{
	static int xs[MAXPT], cocc[GSZ], dx[GSZ];
	static int ys[MAXPT], rocc[GSZ], dy[GSZ];
	int npt=0, c,i,j, x=0,y=0, xmax=0,ymax=0;
	int64_t p1=0,p2=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while ((c = getchar()) != EOF)
		if (c == '\n') {
			xmax = MAX(xmax, x); x=0;
			ymax = y++;
		} else if (c == '#') {
			assert(npt < MAXPT);
			assert(x < GSZ); cocc[x] = 1; xs[npt]   = x++;
			assert(y < GSZ); rocc[y] = 1; ys[npt++] = y;
		} else
			x++;

	for (x=1; x <= xmax; x++) dx[x] = dx[x-1] + !cocc[x-1];
	for (y=1; y <= ymax; y++) dy[y] = dy[y-1] + !rocc[y-1];

	for (i=0; i<npt; i++)
	for (j=0; j<i; j++) {
		p1 += abs(xs[i] +dx[xs[i]] -xs[j] -dx[xs[j]]) +
		      abs(ys[i] +dy[ys[i]] -ys[j] -dy[ys[j]]);
		p2 += abs(xs[i] +dx[xs[i]]*MUL -xs[j] -dx[xs[j]]*MUL) +
		      abs(ys[i] +dy[ys[i]]*MUL -ys[j] -dy[ys[j]]*MUL);
	}

	printf("11: %"PRIi64" %"PRIi64"\n", p1, p2);
	return 0;
}
