#include <stdio.h>
#include <string.h>
#include <assert.h>

#define GSZ	256
#define PAD	96

struct pt { int x,y; };

static char grid[GSZ][GSZ];
static struct pt intent[GSZ][GSZ];
static int claims[GSZ][GSZ];
static int xmin,xmax, ymin,ymax;

static void
update_extent(void)
{
	int x,y;
	
	xmin = ymin = GSZ-1;
	xmax = ymax = 0;

	for (y=0; y<GSZ; y++)
	for (x=0; x<GSZ; x++) {
		if (!grid[y][x]) continue;
		if (y<ymin) ymin = y; if (y>ymax) ymax = y;
		if (x<xmin) xmin = x; if (x>xmax) xmax = x;
	}
}

static void
dump(void)
{
	int x,y;

	printf(" claims\n");
	for (y=ymin; y<=ymax; y++) {
		for (x=xmin; x<=xmax; x++)
			printf(" %1d", claims[y][x]);
		putchar('\n');
	}
	putchar('\n');

	printf(" grid %d,%d..%d,%d\n", xmin,ymin, xmax,ymax);
	for (y=ymin; y<=ymax; y++) {
		for (x=xmin; x<=xmax; x++)
			putchar(grid[y][x] ? '#' : '.');
		putchar('\n');
	}
	putchar('\n');
}

static void
step(int round)
{
	int x,y, ix,iy, d;

	memset(intent, 0, sizeof(intent));
	memset(claims, 0, sizeof(claims));

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++) {
		if (!grid[y][x]) continue;

		ix = x; iy = y;

		for (d=0; d<4; d++)
			switch ((round+d) %4) {
			case 0:
				if (!grid[y-1][x-1] &&
				    !grid[y-1][x] &&
				    !grid[y-1][x+1])
				    	{ iy--; goto have_intent; }
				break;

			case 1:
				if (!grid[y+1][x-1] &&
				    !grid[y+1][x] &&
				    !grid[y+1][x+1])
				    	{ iy++; goto have_intent; }
				break;

			case 2:
				if (!grid[y-1][x-1] &&
				    !grid[y][x-1] &&
				    !grid[y+1][x-1])
				    	{ ix--; goto have_intent; }
				break;

			case 3:
				if (!grid[y-1][x+1] &&
				    !grid[y][x+1] &&
				    !grid[y+1][x+1])
				    	{ ix++; goto have_intent; }
				break;
			}
	have_intent:
		assert(ix); assert(iy);

		intent[y][x].x = ix;
		intent[y][x].y = iy;
		claims[iy][ix]++;
	}

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++) {
		if (!grid[y][x]) continue;

		printf("  %d,%d '%c' %d\n", x,y, grid[y][x], grid[y][x]);

		ix = intent[y][x].x;
		iy = intent[y][x].y;

		printf("I %d,%d '%c' %d\n", ix,iy, grid[iy][ix],
		    grid[iy][ix]);

		assert(claims[iy][ix]);

		if (claims[iy][ix] > 1) continue;

		grid[y][x] = 0;
		grid[iy][ix] = 1;
	}
}

int
main()
{
	int x,y;

	for (y=PAD; fgets(&grid[y][PAD], GSZ-PAD, stdin); y++)
		assert(y < GSZ);
	for (y=0; y<GSZ; y++)
	for (x=0; x<GSZ; x++)
		grid[y][x] = grid[y][x] == '#';

	update_extent();
	dump();

	step(0);
	update_extent();
	dump();

	return 0;
}
