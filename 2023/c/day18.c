#include "common.h"

#define GSZ 768
#define WSZ 640		/* max no. of walls */

/*
 * Conceptually: the raw map, which is too large to fit directly in
 * memory for part 2, is made much smaller by collapsing (and counting)
 * identical rows and columns. Another way to look it at is that a grid
 * is fitted to make 'opaque' cells.
 *                                           |   |#|##|#
 * For example:                             -+---+-+--+-
 *                                          #|###|#|  |#
 *       ####               ### 1           -+---+-+--+-
 *   #####  #             ### # 1           #|   | |  |#
 *   #      #   becomes   #   # 2     or:   #|   | |  |#
 *   #      #             ##### 1           -+---+-+--+-
 *   ########             13121             #|###|#|##|#
 *
 * To avoid a lot of complex work, instead of actually collapsing and
 * splitting rows and columns, we first generate the wall rectangles and
 * collect the unique X and Y coordinates. Those are locations of our
 * virtual grid lines.
 */

enum { BLANK, WALL, OUT };

struct rect { int x,y,w,h; };
struct gridmap { int x[GSZ], y[GSZ], nx, ny; };

static int
cmp_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

static struct rect
to_wall(int x, int y, char dir, int dist)
{
	struct rect ret;

	ret.x = dir=='L' ? x-dist : dir=='R' ? x+1 : x;
	ret.y = dir=='U' ? y-dist : dir=='D' ? y+1 : y;
	ret.w = dir=='L' || dir=='R' ? dist : 1;
	ret.h = dir=='U' || dir=='D' ? dist : 1;

	return ret;
}

static struct rect
map_rect(struct rect a, struct gridmap *m)
{
	struct rect r={0};

	while (r.x < m->nx && m->x[r.x] != a.x) r.x++;
	while (r.y < m->ny && m->y[r.y] != a.y) r.y++;
	while (r.x+r.w < m->nx && m->x[r.x+r.w] != a.x+a.w) r.w++;
	while (r.y+r.h < m->ny && m->y[r.y+r.h] != a.y+a.h) r.h++;

	assert(r.x < m->nx); assert(r.x+r.w < m->nx);
	assert(r.y < m->ny); assert(r.y+r.h < m->ny);

	return r;
}

static void
build_gridmap(const struct rect *walls, int nw, struct gridmap *m)
{
	static int xs[WSZ*2], ys[WSZ*2];
	int i;

	for (i=0; i<nw; i++) {
		xs[i*2] = walls[i].x; xs[i*2+1] = walls[i].x+walls[i].w;
		ys[i*2] = walls[i].y; ys[i*2+1] = walls[i].y+walls[i].h;
	}

	qsort(xs, nw*2, sizeof(*xs), cmp_int);
	qsort(ys, nw*2, sizeof(*ys), cmp_int);

	m->nx = m->ny = 0;

	for (i=0; i<nw*2; i++) {
		assert(m->nx < GSZ);
		assert(m->ny < GSZ);
		if (!i || xs[i] != xs[i-1]) m->x[m->nx++] = xs[i];
		if (!i || ys[i] != ys[i-1]) m->y[m->ny++] = ys[i];
	}
}

static int
flood_step(char grid[][GSZ], int w, int h)
{
	int nchange=0, x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (grid[y][x] == BLANK && (
		    y<=0 || x<=0 || x+1>=w || y+1>=h ||
		    grid[y-1][x] == OUT || grid[y][x-1] == OUT ||
		    grid[y+1][x] == OUT || grid[y][x+1] == OUT)) {
			grid[y][x] = OUT;
			nchange++;
		}

	return nchange;
}

static int64_t
solve(const struct rect *walls, int nw)
{
	static struct gridmap map;
	static char grid[GSZ][GSZ];

	struct rect r;
	int i, x,y;
	int64_t acc=0;

	build_gridmap(walls, nw, &map);

	memset(grid, BLANK, sizeof(grid));

	for (i=0; i<nw; i++) {
		r = map_rect(walls[i], &map);

		for (y = r.y; y < r.y+r.h; y++)
		for (x = r.x; x < r.x+r.w; x++)
			grid[y][x] = WALL;
	}

	while (flood_step(grid, map.nx, map.ny))
		;

	for (y=0; y < map.ny; y++)
	for (x=0; x < map.nx; x++)
		if (grid[y][x] != OUT)
			acc += ((int64_t)map.x[x+1] - map.x[x]) *
			       ((int64_t)map.y[y+1] - map.y[y]);

	return acc;
}

int
main(int argc, char **argv)
{
	static struct rect walls[2][WSZ];	/* part 1, 2 */

	uint64_t p1,p2;
	uint32_t hex;
	int nw=0, x1=0,y1=0, x2=0,y2=0, dist1,dist2;
	char dir1, dir2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	walls[0][0].w = walls[0][0].h = 1;
	walls[1][0].w = walls[1][0].h = 1;
	nw = 1;

	for (; scanf(" %c %d (#%x)", &dir1, &dist1, &hex) == 3; nw++) {
		assert(nw < (int)LEN(*walls));
		dir2 = "RDLU"[hex&0xF];
		dist2 = hex>>4;
		walls[0][nw] = to_wall(x1,y1, dir1, dist1);
		walls[1][nw] = to_wall(x2,y2, dir2, dist2);
		x1 += dir1=='L' ? -dist1 : dir1=='R' ? dist1 : 0;
		x2 += dir2=='L' ? -dist2 : dir2=='R' ? dist2 : 0;
		y1 += dir1=='U' ? -dist1 : dir1=='D' ? dist1 : 0;
		y2 += dir2=='U' ? -dist2 : dir2=='D' ? dist2 : 0;
	}

	p1 = solve(walls[0], nw);
	p2 = solve(walls[1], nw);

	printf("18: %"PRId64" %"PRId64"\n", p1, p2);
	return 0;
}
