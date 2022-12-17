#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "compat/util.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

#define SW	4
#define SH	4
#define GW	7
#define GH	4096
#define SPAWNY	3	/* offset from top */
#define P2	1000000000000

static const char shapes[][SH][SW+1] = {
	{ "....", "....", "....", "####" },
	{ "....", ".#..", "###.", ".#.." },
	{ "....", "..#.", "..#.", "###." },
	{ "#...", "#...", "#...", "#..." },
	{ "....", "....", "##..", "##.." }
};


static char grid[GH][GW];
static int ymin=GH;			/* ymin: stack top */
static int pi, px=2, py=GH-SH-SPAWNY;	/* piece index, pos */

static int min(int a, int b) { return a<b ? a : b; }
//static int max(int a, int b) { return a>b ? a : b; }

static void UNUSED
print_grid(void)
{
	int x,y;

	printf(" top: %d, piece %d at %d,%d\n", GH-ymin, pi, px,py);

	for (y = py-1; y < min(GH, ymin+10); y++) {
		putchar('|');

		for (x=0; x<GW; x++) {
			if (y >= py && y < py+SH &&
			    x >= px && x < px+SW &&
			    shapes[pi][y-py][x-px] == '#')
				putchar('@');
			else if (y >= 0 && grid[y][x])
				putchar('#');
			else
				putchar('.');
		}

		printf("|\n");
	}

	putchar('+');
	for (x=0; x<GW; x++)
		putchar('-');
	printf("+\n\n");
}

static int
hit_test(int dx, int dy)
{
	int x,y;

	assert(py+dy >= 0);

	if (px+dx < 0 || py+dy > GH-SH)
		return 1;

	for (y=0; y<SH; y++)
	for (x=0; x<SW; x++)
		if (shapes[pi][y][x] == '#' && (
		    py+dy+y >= GH ||
		    px+dx+x >= GW ||
		    grid[py+dy+y][px+dx+x]))
			return 1;

	return 0;
}

int
main() {
	static char jets[16*1024];
	int nr, t=0, dx, x,y;
	int64_t nsettled=0;

	nr = (int)fread(jets, 1, sizeof(jets), stdin);
	while (nr>0 && isspace(jets[nr-1]))
		nr--;

	while (nsettled < 1000000000000) {
		switch (jets[t++ % nr]) {
		case '<': dx=-1; break;
		case '>': dx= 1; break;
		default: goto end;
		}

		if (!hit_test(dx, 0))
			px += dx;
		if (!hit_test(0, 1)) {
			/* nothing hit, next step */
			py++;
			continue;
		}

		/* piece settled */
		for (y=0; y<SH; y++)
		for (x=0; x<SW; x++)
			if (px+x >= 0 && px+x < GW &&
			    py+y >= 0 && py+y < GH &&
			    shapes[pi][y][x] == '#') {
				grid[py+y][px+x] = 1;
				ymin = min(ymin, py+y);
			}

		if (++nsettled == 2022) {
			printf("17: %d ", GH-ymin);
			fflush(stdout);
		}

		pi = (pi+1) % (int)LEN(shapes);
		px = 2;
		py = ymin-SH-SPAWNY;
		assert(py >= 0);
	}
end:
	printf("%d\n", GH-ymin);
	return 0;
}
