#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <assert.h>
#include "compat/util.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

#define SW	4
#define SH	4
#define GW	7
#define GH	8192
#define SPAWNY	3	/* offset from top */
#define P2	1000000000000

struct hist { int t, pi, top; int64_t nstack; };

static const char shapes[][SH][SW+1] = {
	{ "....", "....", "....", "####" },
	{ "....", ".#..", "###.", ".#.." },
	{ "....", "..#.", "..#.", "###." },
	{ "#...", "#...", "#...", "#..." },
	{ "....", "....", "##..", "##.." }
};


static char grid[GH][GW];
static struct hist hist[4096];
static size_t nhist;
static int t, top=GH;			/* top: stack top */
static int pi, px=2, py=GH-SH-SPAWNY;	/* piece index, pos */
static int64_t nstack;		/* no. stacked */

static int min(int a, int b) { return a<b ? a : b; }
//static int max(int a, int b) { return a>b ? a : b; }

static void UNUSED
print_grid(void)
{
	int x,y;

	printf(" top: %d, piece %d at %d,%d\n", GH-top, pi, px,py);

	for (y = py-1; y < min(GH, top+10); y++) {
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

static void
hist_push(void)
{
	assert(nhist < LEN(hist));
	hist[nhist].t = t;
	hist[nhist].pi = pi;
	hist[nhist].top = top;
	hist[nhist].nstack = nstack;
	nhist++;
}

/* reverse search first 'lim' of history for t+pi match */
static struct hist *
hist_find(size_t lim)
{
	size_t i;

	for (i=0; i<lim; i++)
		if (hist[lim-i-1].t == t && hist[lim-i-1].pi == pi)
			return &hist[lim-i-1];

	return NULL;
}

static int
find_cycle(int *dtop, int64_t *dnstack)
{
	struct hist *h1=hist+nhist, *h2;

	while ((h1 = h2 = hist_find(h1-hist)))
	while ((h2 = hist_find(h2-hist)))
		if (h1->top - top == h2->top - h1->top &&
		    nstack - h1->nstack == h1->nstack - h2->nstack) {
			*dtop = h1->top - top;
			*dnstack = nstack - h1->nstack;
			return 1;
		}

	return 0;
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
	int nr, dx, x,y;
	int dtop;
	int64_t dnstack, yskip=0, ncycles;

	nr = (int)fread(jets, 1, sizeof(jets), stdin);
	while (nr>0 && isspace(jets[nr-1]))
		nr--;

	for (t=0; nstack < P2; t = (t+1) % nr) {
		switch (jets[t]) {
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
				top = min(top, py+y);
			}

		if (++nstack == 2022) {
			printf("17: %d ", GH-top);
			fflush(stdout);
		}

		if (!yskip) {
			hist_push();

			if (find_cycle(&dtop, &dnstack)) {
				ncycles = (P2-nstack)/dnstack;
				yskip   = ncycles * dtop;
				nstack += ncycles * dnstack;
			}
		}

		pi = (pi+1) % (int)LEN(shapes);
		px = 2;
		py = top-SH-SPAWNY;
		assert(py >= 0);
	}
end:
	printf("%" PRId64 "\n", yskip + GH-top);
	return 0;
}
