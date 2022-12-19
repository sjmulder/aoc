#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))
#define NRES	4

/*
 * VERY BROKEN solution. Wanted to see if hardcoding the variables
 * and robot-specific logic would make a difference. Wildly wrong
 * answers at this point.
 */

struct bp { int ore_ore, cly_ore, obs_ore,obs_cly, geo_ore,geo_obs; };
struct st { int t, ore,cly,obs,geo, ore_ro,cly_ro,obs_ro,geo_ro; };

struct bp bps[32];
size_t nbp;

static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

static void
recur(struct bp *bp, struct st st, int *top)
{
	struct st st2;

	/* base case: do nothing */
	*top = max(*top, st.geo + st.t*st.geo_ro);

	/* prune time spent looking at pointless t=end decisions */
	if (st.t <= 1)
		return;

	/*
	 * Prune by testing upper limit, thanks /u/Boojum!
	 * https://www.reddit.com/r/adventofcode/comments/zpihwi/2022_day_19_solutions/j0tls7a/
	 */
	if (st.geo + st.t*st.geo_ro + st.t*(st.t+1)/2 < *top)
		return;

	/* build geode robot? */
	if (st.obs_ro) {
		st2 = st;
		while (st2.ore < bp->geo_ore && st2.obs < bp->geo_obs) {
			st2.t--;
			st2.ore += st2.ore_ro;
			st2.cly += st2.cly_ro;
			st2.obs += st2.obs_ro;
			st2.geo += st2.geo_ro;
		}
		st2.t--;
		st2.ore -= bp->geo_ore;
		st2.obs -= bp->geo_obs;
		st2.geo_ro++;

		recur(bp, st2, top);
	}

	/* build obsidian robot? */
	if (st.cly_ro && st.obs_ro < bp->geo_obs) {
		st2 = st;
		while (st2.ore < bp->obs_ore && st2.cly < bp->obs_cly) {
			st2.t--;
			st2.ore += st2.ore_ro;
			st2.cly += st2.cly_ro;
			st2.obs += st2.obs_ro;
			st2.geo += st2.geo_ro;
		}
		st2.t--;
		st2.ore -= bp->obs_ore;
		st2.cly -= bp->obs_cly;
		st2.obs_ro++;

		recur(bp, st2, top);
	}

	/* build clay robot? */
	if (st.cly_ro < bp->obs_cly) {
		st2 = st;
		while (st2.ore < bp->cly_ore) {
			st2.t--;
			st2.ore += st2.ore_ro;
			st2.cly += st2.cly_ro;
			st2.obs += st2.obs_ro;
			st2.geo += st2.geo_ro;
		}
		st2.t--;
		st2.ore -= bp->cly_ore;
		st2.cly_ro++;

		recur(bp, st2, top);
	}

	/* build ore robot? */
	if (st.ore_ro < bp->cly_ore ||
	    st.ore_ro < bp->obs_ore ||
	    st.ore_ro < bp->geo_ore) {
		st2 = st;
		while (st2.ore < bp->ore_ore) {
			st2.t--;
			st2.ore += st2.ore_ro;
			st2.cly += st2.cly_ro;
			st2.obs += st2.obs_ro;
			st2.geo += st2.geo_ro;
		}
		st2.t--;
		st2.ore -= bp->ore_ore;
		st2.ore_ro++;

		recur(bp, st2, top);
	}
}

int
main()
{
	struct st st;
	int i, top, p1=0, p2=1;

	while (6 == scanf(
" Blueprint %*d:"
" Each ore robot costs %d ore."
" Each clay robot costs %d ore."
" Each obsidian robot costs %d ore and %d clay."
" Each geode robot costs %d ore and %d obsidian.",
	    &bps[nbp].ore_ore,
	    &bps[nbp].cly_ore,
	    &bps[nbp].obs_ore, &bps[nbp].obs_cly,
	    &bps[nbp].geo_ore, &bps[nbp].geo_obs)) {
		nbp++;
		assert(nbp < LEN(bps));
	}

	memset(&st, 0, sizeof(st));
	st.ore_ro = 1;

	for (i=0; i < (int)nbp; i++) {
		st.t = 24;
		top = 0;
		recur(&bps[i], st, &top);
		p1 += (i+1)*top;
	}

	for (i=0; i < min((int)nbp, 3); i++) {
		st.t = 32;
		top = 0;
		recur(&bps[i], st, &top);
		p2 *= top;
	}

	printf("19: %d %d\n", p1, p2);
	return 0;
}
