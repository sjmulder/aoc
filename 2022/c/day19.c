#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))
#define NRES	4

enum { GEODE, OBSID, CLAY, ORE };
struct bp { int price[NRES][NRES]; };
struct st { struct bp *bp; int t, res[NRES], robos[NRES]; };

struct bp bps[32];
size_t nbp;

static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

static int
recur(struct st st)
{
	struct st st2;
	int i,j, tbuy, best=0, maxp;

	if (st.t <= 1)
		return st.res[GEODE] + st.robos[GEODE]*st.t;

	/* option 1: do nothing (if we make GEODE) */
	best = st.res[GEODE] + st.t*st.robos[GEODE];

	/* option 2[i]: save up for new robot of type i */
	for (i=0; i<NRES; i++) {
		/* only if we don't already make enough of that */
		if (i != GEODE) {
			for (j=0, maxp=0; j<NRES; j++)
				maxp = max(maxp, st.bp->price[j][i]);
			if (i && st.robos[i] >= maxp)
				continue;
		}

		/* can buy, and how long? */
		tbuy = 1;
		for (j=0; j<NRES; j++) {
			if (st.res[j] >= st.bp->price[i][j]) continue;
			if (!st.robos[j]) goto nobuy;
			tbuy = max(tbuy,
			    (st.bp->price[i][j] - st.res[j] +
			     st.robos[j]-1) / st.robos[j] +1);
			if (tbuy >= st.t) goto nobuy;
		}

		/* skip ahead and buy */
		st2 = st;
		st2.t -= tbuy;
		for (j=0; j<NRES; j++) {
			st2.res[j] += st2.robos[j] * tbuy;
			st2.res[j] -= st2.bp->price[i][j];
		}
		st2.robos[i]++;

		best = max(best, recur(st2));
	nobuy: ;
	}

	return best;
}

int
main()
{
	struct st st;
	int i, val, p1=0, p2=1;

	while (6 == scanf(
" Blueprint %*d:"
" Each ore robot costs %d ore."
" Each clay robot costs %d ore."
" Each obsidian robot costs %d ore and %d clay."
" Each geode robot costs %d ore and %d obsidian.",
	    &bps[nbp].price[ORE][ORE],
	    &bps[nbp].price[CLAY][ORE],
	    &bps[nbp].price[OBSID][ORE],
	    &bps[nbp].price[OBSID][CLAY],
	    &bps[nbp].price[GEODE][ORE],
	    &bps[nbp].price[GEODE][OBSID])) {
		nbp++;
		assert(nbp < LEN(bps));
	}

	memset(&st, 0, sizeof(st));
	st.robos[ORE] = 1;

	for (i=0; i < (int)nbp; i++) {
		st.t = 24;
		st.bp = &bps[i];
		p1 += (i+1) * (val = recur(st));
	}

	for (i=0; i < min((int)nbp, 3); i++) {
		st.t = 32;
		st.bp = &bps[i];
		p2 *= (val = recur(st));
	}

	printf("19: %d %d\n", p1, p2);
	return 0;
}