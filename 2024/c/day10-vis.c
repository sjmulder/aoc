#include "common.h"
#include "vis.h"

#define GZ	43
#define VISGZ	40	/* how much of the grid */
#define CELLZ	20	/* pixels per cell */
#define SPEED	1	/* frames per step */

static char g[GZ][GZ];
static int seen[GZ][GZ];

static struct vis vis;

static int
score(int id, int x, int y, int p2, int depth)
{
	uint8_t rr, gg, bb;
	int ret;

	if (x<0 || x>=GZ ||
	    y<0 || y>=GZ || (!p2 && seen[y][x] == id))
		return 0;

	seen[y][x] = id;

	lerp_rainbow(id % 8 * 0.12, &rr, &gg, &bb);
	rr = 128 + rr/2 - 12*depth;
	gg = 128 + gg/2 - 12*depth;
	bb = 128 + bb/2 - 12*depth;

	/* highlight current tile in white */
	vis_fill(&vis, x*CELLZ, y*CELLZ, CELLZ, CELLZ, 255, 255, 255);
	vis_emit(&vis, SPEED);
	
	/* then the proper color for the next frame */
	vis_fill(&vis, x*CELLZ, y*CELLZ, CELLZ, CELLZ, rr, gg, bb);

	/* draw little square at starting point */
	if (!depth)
		vis_fill(&vis,
		    x*CELLZ + CELLZ/4,
		    y*CELLZ + CELLZ/4, CELLZ/2, CELLZ/2,
		    255, 255, 255);

	if (g[y][x] == '9')
		return 1;

	ret =
	    (g[y-1][x] == g[y][x]+1 ? score(id, x, y-1, p2, depth+1) : 0) +
	    (g[y+1][x] == g[y][x]+1 ? score(id, x, y+1, p2, depth+1) : 0) +
	    (g[y][x-1] == g[y][x]+1 ? score(id, x-1, y, p2, depth+1) : 0) +
	    (g[y][x+1] == g[y][x]+1 ? score(id, x+1, y, p2, depth+1) : 0);

	/* clear the little square */
	if (!depth)
		vis_fill(&vis, x*CELLZ, y*CELLZ, CELLZ, CELLZ, rr, gg, bb);

	return ret;
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, id=1, x,y;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	for (y=0; y<GZ && fgets(g[y], GZ, stdin); y++)
		;
	
	vis_begin(&vis, "day10.mp4", 60, VISGZ*CELLZ, VISGZ*CELLZ);

	for (y=0; y<VISGZ; y++)
	for (x=0; x<VISGZ; x++)
		vis_fill(&vis, x*CELLZ, y*CELLZ, CELLZ, CELLZ,
		    0, (g[y][x]-'0') * 6, 0);
	
	vis_emit(&vis, SPEED);

	for (y=0; y<GZ; y++)
	for (x=0; x<GZ; x++)
		if (g[y][x] == '0') {
			p1 += score(id++, x, y, 0, 0);
			p2 += score(id++, x, y, 1, 0);
		}

	vis_emit(&vis, 30);

	printf("10: %d %d\n", p1, p2);
	return 0;
}
