#include "common.h"

#define SAMPLE	0
#define GZ	(SAMPLE ? 9 : 73)
#define NCORR	(SAMPLE ? 12 : 1024)
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
	int p1=0, x,y, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (i=0; i<GZ; i++)
		g[0][i] = g[GZ-1][i] =
		g[i][0] = g[i][GZ-1] = CORR;
	
	g[1][1] = 1;

	for (i=0; scanf(" %d,%d", &x, &y) == 2; i++) {
		assert(x >= 0); assert(x < GZ-2);
		assert(y >= 0); assert(y < GZ-2);
		g[y+1][x+1] = CORR;

		if (i < NCORR-1)
			continue;

		flood();

		if (i==NCORR-1)
			p1 = g[GZ-2][GZ-2]-1;
		if (g[GZ-2][GZ-2] <= 0) {
			printf("18: %d %d,%d\n", p1, x,y);
			return 0;
		}
	}

	assert(!"no solution");
	return -1;
}
