#include "common.h"

#define SAMPLE	0
#define PTZ	3600
#define GZ	(SAMPLE ? 9 : 73)
#define P1STEP	(SAMPLE ? 12 : 1024)
#define CORR	-1

static int g[GZ][GZ];

static void
flood(int x, int y)
{
	int lo=INT_MAX;

	if (x <= 0 || x >= GZ-1 ||
	    y <= 0 || y >= GZ-1 || g[y][x] == CORR)
		return;

	if (g[y-1][x] > 0) lo = MIN(lo, g[y-1][x] +1);
	if (g[y+1][x] > 0) lo = MIN(lo, g[y+1][x] +1);
	if (g[y][x-1] > 0) lo = MIN(lo, g[y][x-1] +1);
	if (g[y][x+1] > 0) lo = MIN(lo, g[y][x+1] +1);

	if (lo != INT_MAX && (!g[y][x] || g[y][x] > lo)) {
		g[y][x] = lo;

		flood(x, y-1);
		flood(x, y+1);
		flood(x-1, y);
		flood(x+1, y);
	}
}

int
main(int argc, char **argv)
{
	static int xs[PTZ], ys[PTZ];
	static char p2[32];
	int p1=0, npt=0, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (i=0; i<GZ; i++)
		g[0][i] = g[GZ-1][i] =
		g[i][0] = g[i][GZ-1] = CORR;

	for (npt=0; npt<PTZ && scanf(" %d,%d", xs+npt, ys+npt)==2; npt++) {
		assert(xs[npt] >= 0); assert(xs[npt] < GZ-2);
		assert(ys[npt] >= 0); assert(ys[npt] < GZ-2);
	}

	assert(npt < PTZ);

	for (i=0; i<npt; i++)
		g[ys[i]+1][xs[i]+1] = CORR;

	g[1][1] = 1;
	flood(2, 1);
	flood(1, 2);

	for (i=npt-1; i >= P1STEP; i--) {
		g[ys[i]+1][xs[i]+1] = 0;
		flood(xs[i]+1, ys[i]+1);

		if (!p2[0] && g[GZ-2][GZ-2] > 0)
			snprintf(p2, sizeof(p2), "%d,%d", xs[i],ys[i]);
	}

	p1 = g[GZ-2][GZ-2]-1;

	printf("18: %d %s\n", p1, p2);
	return 0;
}
