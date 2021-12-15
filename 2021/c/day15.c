#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define SZ 100

struct pt { int x,y; };
struct node { int x,y, open, g; };
static struct node nodes[SZ*5][SZ*5];

static int grid[SZ][SZ];
static int gridw, gridh;

static void
dump_grid(void)
{
	int x,y;

	for (y=0; y<gridh*5; y++)  {
		for (x=0; x<gridw*5; x++)
			putchar('0' + (
			    grid[y % gridh][x % gridw] +
			    x/gridw + y/gridh -1) %9 +1);
		putchar('\n');
	}
}

static void
dump_nodes(void)
{
	int x,y;

	for (y=0; y<gridh; y++)  {
		for (x=0; x<gridw; x++)
			if (nodes[y][x].g == INT_MAX)
				printf(" * ");
			else
				printf("%2d ", nodes[y][x].g);
		putchar('\n');
	}
}

static int
astar(int startx, int starty, int endx, int endy)
{
	struct node *cur;
	int x,y, i, f,fmin, g;

	memset(nodes, 0, sizeof(nodes));

	for (y=0; y<gridh*5; y++)
	for (x=0; x<gridw*5; x++) {
		nodes[y][x].x = x;
		nodes[y][x].y = y;
		nodes[y][x].g = INT_MAX;
	}

	nodes[starty][startx].open = 1;
	nodes[starty][startx].g = 0;

	while (1) {
		cur = NULL;
		fmin = INT_MAX;

		for (y=0; y<gridh*5; y++)
		for (x=0; x<gridw*5; x++) {
			if (!nodes[y][x].open) continue;
			f = nodes[y][x].g + abs(x-endx) + abs(y-endy);
			if (f < fmin) { fmin = f; cur = &nodes[y][x]; }
		}

		if (!cur) return -1;
		if (cur->x == endx && cur->y == endy) return cur->g;

		cur->open = 0;
		//printf("  at (%d,%d) g=%d\n", cur->y, cur->x, cur->g);

		for (i=0; i<4; i++) {
			y = cur->y + (i==0?-1 : i==2? 1 : 0);
			x = cur->x + (i==1? 1 : i==3?-1 : 0);
			if (y < 0 || y > endy) continue;
			if (x < 0 || x > endx) continue;
			if (x == cur->x && y == cur->y) continue;

			g = cur->g + (
			     grid[y % gridh][x % gridw] +
			     x/gridw + y/gridh -1) %9 +1;

			if (g >= nodes[y][x].g) continue;

			nodes[y][x].open = 1;
			nodes[y][x].g = g;
			//printf("  opened (%d,%d) g=%d\n", x,y, g);
		}
	}
}

int
main()
{
	int x=0,y=0, c, p1,p2;

	while ((c = getchar()) != EOF) {
		if (c == '\n') { x=0; y++; continue; }
		assert(x < SZ);
		assert(y < SZ);
		grid[y][x] = c-'0';
		if (y >= gridh) gridh = y+1;
		if (x >= gridw) gridw = x+1;
		x++;
	}

	p1 = astar(0,0, gridw  -1, gridh  -1);
	p2 = astar(0,0, gridw*5-1, gridh*5-1);

	//dump_grid();
	//dump_nodes();

	printf("14: %d %d\n", p1, p2);
	return 0;
}
