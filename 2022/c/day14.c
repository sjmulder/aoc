#include "common.h"

#ifdef WITH_VIS
#include <stdint.h>
#include <unistd.h>
#include "vis.h"
#endif

#define HSZ	512
#define VSZ	256
#define XOFF	192
#define XDROP	(500-XOFF)

static char map[VSZ][HSZ];
static int xmin=HSZ-1, xmax, ymax;

static int max(int a, int b) { return a>b ? a : b; }
static int sgn(int x) { return x<0 ? -1 : x>0 ? 1 : 0; }

static void UNUSED
dump(void)
{
	int x,y;

	printf(" x=%d..%d y=0..%d\n", xmin,xmax, ymax);

	for (y=0; y <= ymax+1; y++) {
		for (x = xmin-1; x <= xmax+1; x++)
			putchar(map[y][x] ? map[y][x] : '.');
		putchar('\n');
	}
}

static void
draw_rocks(int x0, int y0, int x1, int y1)
{
	int n, dx,dy, i;

	n = max(abs(x1 - x0), abs(y1 - y0)) +1;
	dx = sgn(x1 - x0);
	dy = sgn(y1 - y0);

	for (i=0; i<n; i++)
		map[y0 + dy*i][x0 + dx*i] = '#';
}

static void
read_input(void)
{
	int x0=0,x1, y0=0,y1, is_first=1;

	while (scanf(" %d,%d", &x1, &y1) == 2) {
		x1 -= XOFF;

		assert(x1 > 0); assert(x1+1 < HSZ);
		assert(y1 > 0); assert(y1+1 < VSZ);

		if (x1 < xmin) xmin = x1;
		if (x1 > xmax) xmax = x1;
		if (y1 > ymax) ymax = y1;

		if (!is_first)
			draw_rocks(x0,y0, x1,y1);

		x0 = x1;
		y0 = y1;

		if (!(is_first = (getchar() == '\n')))
			(void)!scanf(" ->");
	}
}

static void
drop_sand(int *hit_floor)
{
	int x=XDROP, y;

	for (y=0; y < ymax+1; y++) {
		if (!map[y+1][x]) ;
		else if (!map[y+1][x-1]) x--;
		else if (!map[y+1][x+1]) x++;
		else break;
	}

	map[y][x] = *hit_floor ? 'o' : 'O';

	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y >= ymax) *hit_floor = 1;
}

#ifdef WITH_VIS
static struct vis vis;

static struct vis_grid vis_grid = {
	.w = HSZ,
	.h = VSZ,
	.cell_sz = 2,
	.data = (uint8_t *)map,
	.colors = {
		['O'] = {255, 196, 0},
		['o'] = {255, 128, 0},
		['#'] = {255, 255, 255}
	}
};

void
vis14_begin(void)
{
	vis_grid.h = ymax+2;

	vis_begin(&vis, "day14.mp4", 120,
	    vis_grid.w * vis_grid.cell_sz,
	    vis_grid.h * vis_grid.cell_sz);
}

void
vis14_emit(void)
{
	vis_draw_grid(&vis, &vis_grid);
	vis_emit(&vis, 1);
}

void
vis14_end(void)
{
	vis_emit(&vis, 120);
	vis_end(&vis);
}
#else
void vis14_begin(void) {}
void vis14_emit(void) {}
void vis14_end(void) {}
#endif

int
main(int argc, char **argv)
{
	int p1=0,p2=0, hit_floor=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	read_input();
	vis14_begin();

	while (!map[0][XDROP]) {
		drop_sand(&hit_floor);
		p1 += !hit_floor;
		p2 += 1;

		if (!hit_floor || p2%10 == 0)
		    vis14_emit();
	}

	vis14_end();
	printf("14: %d %d\n", p1, p2);

	return 0;
}
