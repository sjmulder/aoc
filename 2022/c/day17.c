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

static int min(int a, int b) { return a<b ? a : b; }

static void
hist_push(int t, int pi, int top, int64_t nstack)
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
hist_find(size_t lim, int t, int pi)
{
	size_t i;

	for (i=0; i<lim; i++)
		if (hist[lim-i-1].t == t && hist[lim-i-1].pi == pi)
			return &hist[lim-i-1];

	return NULL;
}

/* find game state cycle based on current hist top */
static int
find_cycle(int *dtop, int64_t *dnstack)
{
	struct hist *h1,*h2,*h3;

	h1 = h2 = &hist[nhist-1];

	while ((h2 = h3 = hist_find(h2-hist, h1->t, h1->pi)))
	while ((     h3 = hist_find(h3-hist, h1->t, h1->pi)))
		if (h1->top - h2->top ==
		    h2->top - h3->top &&
		    h2->nstack - h1->nstack ==
		    h3->nstack - h2->nstack) {
			*dtop    = h2->top - h1->top;
			*dnstack = h1->nstack - h2->nstack;
			return 1;
		}

	return 0;
}

static int
hit_test(int pi, int px, int py)
{
	int x,y;

	assert(py >= 0);

	if (px < 0 || py > GH-SH)
		return 1;

	for (y=0; y<SH; y++)
	for (x=0; x<SW; x++)
		if (shapes[pi][y][x] == '#' &&
		    (py+y >= GH || px+x >= GW || grid[py+y][px+x]))
			return 1;

	return 0;
}

int
main()
{
	static char jets[16*1024];
	int top=GH, t, nr, dx, x,y, dtop;
	int pi=0, px=2, py=GH-SH-SPAWNY;
	int64_t p1=0, nstack=0, yskip=0, ncycles, dnstack;

	nr = (int)fread(jets, 1, sizeof(jets), stdin);
	while (nr>0 && isspace(jets[nr-1]))
		nr--;

	for (t=0; nstack < P2; t = (t+1) % nr) {
		dx = (jets[t] == '>') *2-1;

		if (!hit_test(pi, px+dx, py))
			px += dx;
		if (!hit_test(pi, px, py+1)) {
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

		if (++nstack == 2022)
			p1 = GH-top;

		if (!yskip) {
			hist_push(t, pi, top, nstack);

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

	printf("17: %"PRId64" %"PRId64"\n", p1, yskip+GH-top);
	return 0;
}
