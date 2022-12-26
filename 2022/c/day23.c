#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

#define GSZ	256
#define PAD	96

struct pt { int x,y; };
struct move { struct pt f, t; };	/* from, to */

static char grid[GSZ][GSZ];

static void vis23_begin(void);
static void vis23_emit(void);
static void vis23_end(void);

/* returns number moved */
static int
step(int round)
{
	static struct move moves[GSZ*GSZ/2];
	static int claims[GSZ][GSZ];
	int x,y,i, nm=0, ret=0; /* nm: number of moves[] */

	memset(moves, 0, sizeof(moves));
	memset(claims, 0, sizeof(claims));


	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++) {
		if (!grid[y][x]) continue;

		grid[y][x] = 1;

		if (!grid[y-1][x-1] &&
		    !grid[y-1][x  ] &&
		    !grid[y-1][x+1] &&
		    !grid[y  ][x-1] &&
		    !grid[y  ][x+1] &&
		    !grid[y+1][x-1] &&
		    !grid[y+1][x  ] &&
		    !grid[y+1][x+1])
			continue;

		assert((size_t)nm < LEN(moves));

		moves[nm].f.x = moves[nm].t.x = x;
		moves[nm].f.y = moves[nm].t.y = y;

		for (i=0; i<4; i++)
			switch ((round+i) %4) {
			case 0:
				if (!grid[y-1][x-1] &&
				    !grid[y-1][x  ] &&
				    !grid[y-1][x+1]) {
					moves[nm++].t.y = y-1;
					grid[y][x] = 2;
					goto break_i;
				}
				break;

			case 1:
				if (!grid[y+1][x-1] &&
				    !grid[y+1][x  ] &&
				    !grid[y+1][x+1]) {
					moves[nm++].t.y = y+1;
					grid[y][x] = 3;
					goto break_i;
				}
				break;

			case 2:
				if (!grid[y-1][x-1] &&
				    !grid[y  ][x-1] &&
				    !grid[y+1][x-1]) {
					moves[nm++].t.x = x-1;
					grid[y][x] = 4;
					goto break_i;
				}
				break;

			case 3:
				if (!grid[y-1][x+1] &&
				    !grid[y  ][x+1] &&
				    !grid[y+1][x+1]) {
					moves[nm++].t.x = x+1;
					grid[y][x] = 5;
					goto break_i;
				}
				break;
			}
	break_i: ;
	}

	vis23_emit();

	for (i=0; i<nm; i++)
		claims[moves[i].t.y][moves[i].t.x]++;

	for (i=0; i<nm; i++) {
		if (claims[moves[i].t.y][moves[i].t.x] > 1)
			continue;

		grid[moves[i].t.y][moves[i].t.x] =
		    grid[moves[i].f.y][moves[i].f.x];
		grid[moves[i].f.y][moves[i].f.x] = 0;

		ret++;
	}

	vis23_emit();
	return ret;
}

static int
count_empty(void)
{
	int x,y, xmin,xmax, ymin,ymax, n=0;

	xmin = ymin = GSZ-1;
	xmax = ymax = 0;

	for (y=0; y<GSZ; y++)
	for (x=0; x<GSZ; x++) {
		if (!grid[y][x]) continue;
		if (y<ymin) {ymin = y;} if (y>ymax) {ymax = y;}
		if (x<xmin) {xmin = x;} if (x>xmax) {xmax = x;}
	}

	for (y=ymin; y<=ymax; y++)
	for (x=xmin; x<=xmax; x++)
		n += !grid[y][x];

	return n;
}

int
main(int argc, char **argv)
{
	int x,y, i, p1;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	for (y=PAD; fgets(&grid[y][PAD], GSZ-PAD, stdin); y++)
		assert(y < GSZ);
	for (y=0; y<GSZ; y++)
	for (x=0; x<GSZ; x++)
		grid[y][x] = grid[y][x] == '#';

	vis23_begin();

	for (i=0; i<10; i++)
		step(i);

	p1 = count_empty();

	while (step(i))
		i++;

	vis23_end();

	printf("23: %d %d\n", p1, i+1);
	return 0;
}

#ifdef WITH_VIS
#include <stdint.h>
#include <unistd.h>
#include "vis.h"

#define SCALE	5
#define SLOW	2

static struct vis vis;

static struct vis_grid vis_grid = {
	.data = (uint8_t *)grid,
	.colors = {
		//[0] = { 64, 64, 64},
		[1] = {128,128,128},
		[2] = {196, 64, 64},
		[3] = {255,255,  0}, 
		[4] = { 64,196, 64},
		[5] = {  0,255,255} 
	},
	.w = GSZ,
	.h = GSZ,
	.cell_sz = SCALE
};

static void
vis23_begin(void)
{
	vis_begin(&vis, "day23.mp4", 60, SCALE*GSZ, SCALE*GSZ);
}

static void
vis23_emit(void)
{
	vis_draw_grid(&vis, &vis_grid);
	vis_emit(&vis, SLOW);
}

static void
vis23_end(void)
{
	vis_end(&vis);
}

#else
static void vis23_begin(void) {}
static void vis23_emit(void) {}
static void vis23_end(void) {}
#endif
