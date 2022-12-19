#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))
#define NRES	4

enum { GEO, OBS, CLY, ORE };
struct bp { int price[NRES][NRES]; };
struct st { int t, res[NRES], robos[NRES]; };

struct bp bps[32];
size_t nbp;

static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

static int
recur(struct bp *bp, struct st st, int best)
{
	struct st st2;
	int i,j, tbuy, maxp;

	/* option 1: do nothing (if we make GEO) */
	best = max(best, st.res[GEO] + st.t*st.robos[GEO]);

	/* prune time spent looking at pointless t=end decisions */
	if (st.t <= 1)
		return best;

	/*
	 * Prune by testing upper limit, thanks /u/Boojum!
	 * https://www.reddit.com/r/adventofcode/comments/zpihwi/2022_day_19_solutions/j0tls7a/
	 */
	if (st.res[GEO] + st.t*st.robos[GEO] + st.t*(st.t+1)/2 < best)
		return best;

	/* option 2[i]: (save up for and) buy new robot of type i */
	for (i=0; i<NRES; i++) {
		/* only if we don't already make enough of that */
		if (i != GEO) {
			for (j=0, maxp=0; j<NRES; j++)
				maxp = max(maxp, bp->price[j][i]);
			if (st.robos[i] >= maxp)
				continue;
		}

		/* can buy, and how long? */
		tbuy = 1;
		for (j=0; j<NRES; j++) {
			if (st.res[j] >= bp->price[i][j]) continue;
			if (!st.robos[j]) goto nobuy;
			tbuy = max(tbuy,
			    (bp->price[i][j] - st.res[j] +
			     st.robos[j]-1) / st.robos[j] +1);
			if (tbuy >= st.t) goto nobuy;
		}

		/* skip ahead and buy */
		st2 = st;
		st2.t -= tbuy;
		for (j=0; j<NRES; j++) {
			st2.res[j] += st2.robos[j] * tbuy;
			st2.res[j] -= bp->price[i][j];
		}
		st2.robos[i]++;

		best = max(best, recur(bp, st2, best));
	nobuy: ;
	}

	return best;
}

int
main()
{
	struct st st;
	int i, p1=0, p2=1;

	while (6 == scanf(
" Blueprint %*d:"
" Each ore robot costs %d ore."
" Each clay robot costs %d ore."
" Each obsidian robot costs %d ore and %d clay."
" Each geode robot costs %d ore and %d obsidian.",
	    &bps[nbp].price[ORE][ORE], &bps[nbp].price[CLY][ORE],
	    &bps[nbp].price[OBS][ORE], &bps[nbp].price[OBS][CLY],
	    &bps[nbp].price[GEO][ORE],
	    &bps[nbp].price[GEO][OBS])) {
		nbp++;
		assert(nbp < LEN(bps));
	}

	memset(&st, 0, sizeof(st));
	st.robos[ORE] = 1;
	st.t = 24;

	for (i=0; i < (int)nbp; i++)
		p1 += (i+1) * recur(&bps[i], st, 0);

	st.t = 32;
	for (i=0; i < min((int)nbp, 3); i++)
		p2 *= recur(&bps[i], st, 0);

	printf("19: %d %d\n", p1, p2);
	return 0;
}
