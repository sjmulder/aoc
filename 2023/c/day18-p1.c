#include "common.h"

#define GSZ 1600

enum { BLANK, OUT, WALL };

static char grid[GSZ][GSZ];
static int xmin,xmax, ymin,ymax;

static int
flood(void)
{
	int nchange=0, x,y;

	for (y=ymin-1; y<=ymax+1; y++)
	for (x=xmin-1; x<=xmax+1; x++)
		if (grid[y][x] == BLANK && (
		    grid[y-1][x] == OUT ||
		    grid[y+1][x] == OUT ||
		    grid[y][x-1] == OUT ||
		    grid[y][x+1] == OUT)) {
			grid[y][x] = OUT;
			nchange++;
		}
	
	return nchange;
}

int
main(int argc, char **argv)
{
	char dir;
	int p1=0, x=GSZ/2,y=GSZ/2, dist, i;
	unsigned color;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	xmin = ymin = GSZ-1;
	xmax = ymax = 0;

	while (scanf(" %c %d (#%x)", &dir, &dist, &color) == 3) {
		for (i=0; i<dist; i++) {
			x += dir=='R' ? 1 : dir=='L' ? -1 : 0;
			y += dir=='D' ? 1 : dir=='U' ? -1 : 0;
			xmin = MIN(xmin, x); xmax = MAX(xmax, x);
			ymin = MIN(ymin, y); ymax = MAX(ymax, y);
			assert(x>0); assert(x < GSZ-1);
			assert(y>0); assert(y < GSZ-1);
			grid[y][x] = WALL;
		}
	}

	grid[ymin-1][xmin-1] = OUT;
	while (flood())
		;

	for (y=ymin; y<=ymax; y++)
	for (x=xmin; x<=xmax; x++)
		p1 += grid[y][x] != OUT;

	printf("18: %d\n", p1);
	return 0;
}
