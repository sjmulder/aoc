#include "common.h"

#define SAMPLE	0
#define PTZ	3600
#define GZ	(SAMPLE ? 9 : 73)
#define P1STEP	(SAMPLE ? 12 : 1024)
#define CORR	-1

static int g[GZ][GZ];

static int
flood_step(int x, int y)
{
	int lo=INT_MAX;

	if (g[y][x] == CORR) return 0;

	if (g[y-1][x] > 0) lo = MIN(lo, g[y-1][x] +1);
	if (g[y+1][x] > 0) lo = MIN(lo, g[y+1][x] +1);
	if (g[y][x-1] > 0) lo = MIN(lo, g[y][x-1] +1);
	if (g[y][x+1] > 0) lo = MIN(lo, g[y][x+1] +1);

	if (lo != INT_MAX && (!g[y][x] || g[y][x] > lo))
		{ g[y][x] = lo; return 1; }

	return 0;
}

static void
flood(void)
{
	int x,y, dirty=1;

	for (y=1; y<GZ-1; y++)
	for (x=1; x<GZ-1; x++)
		if (g[y][x] > 1)
			g[y][x] = 0;

	while (dirty) {
		dirty = 0;

		for (y=1; y<GZ-1; y++)
		for (x=1; x<GZ-1; x++)
			dirty = flood_step(x, y) || dirty;

		for (y=GZ-2; y>=1; y--)
		for (x=GZ-2; x>=1; x--)
			dirty = flood_step(x, y) || dirty;
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
	flood();

	for (i=npt-1; i >= P1STEP; i--) {
		g[ys[i]+1][xs[i]+1] = 0;

		if (!flood_step(xs[i]+1, ys[i]+1))
			continue;

		flood();

		if (!p2[0] && g[GZ-2][GZ-2] > 0)
			snprintf(p2, sizeof(p2), "%d,%d", xs[i],ys[i]);
	}

	p1 = g[GZ-2][GZ-2]-1;

	printf("18: %d %s\n", p1, p2);
	return 0;
}
