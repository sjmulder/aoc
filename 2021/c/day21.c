#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/*
 * To halve the required cache size, 'state' does not include a 'current
 * player' field, instead that aspect is encoded in the order - pos0
 * score0 are the current player, pos1 and score1 the other.
 *
 * apply_throw() and add_res() resp. apply and reverse the flip.
 *
 * Credits to /u/4HbQ for the insight.
 */

struct state { int pos0,score0, pos1,score1; };
struct p2 { uint64_t wins0, wins1; char set; };

static struct state
apply_throw(struct state st0, int throw)
{
	struct state st;

	memset(&st, 0, sizeof(st));
	st.pos0 = st0.pos1;
	st.pos1 = (st0.pos0 + throw) % 10;
	st.score0 = st0.score1;
	st.score1 = st0.score0 + st.pos1+1;

	return st;
}

static void
add_res(struct p2 *acc, struct p2 res, int count)
{
	acc->wins0 += res.wins1 * count;
	acc->wins1 += res.wins0 * count;
}

static int
solve_p1(struct state st)
{
	int nthrows=0, throw, i;

	while (st.score1 < 1000) {
		for (throw=0, i=0; i<3; i++)
			throw += nthrows++ % 100 +1;

		st = apply_throw(st, throw);
	}

	return MIN(st.score0, st.score1) * nthrows;
}

static struct p2
solve_p2(struct state s)
{
	static struct p2 cache[10][21][10][21];
	struct p2 res, *ent;

	ent = &cache[s.pos0][s.score0][s.pos1][s.score1];
	memset(&res, 0, sizeof(res));

	if (s.score1 >= 21) {
		res.wins1 = 1;
		return res;
	} else if (ent->set) {
		return *ent;
	} else {
		add_res(&res, solve_p2(apply_throw(s, 3)), 1);
		add_res(&res, solve_p2(apply_throw(s, 4)), 3);
		add_res(&res, solve_p2(apply_throw(s, 5)), 6);
		add_res(&res, solve_p2(apply_throw(s, 6)), 7);
		add_res(&res, solve_p2(apply_throw(s, 7)), 6);
		add_res(&res, solve_p2(apply_throw(s, 8)), 3);
		add_res(&res, solve_p2(apply_throw(s, 9)), 1);

		res.set = 1;
		return *ent = res;
	}
}

int
main(void)
{
	struct state st;
	int p1, n;
	struct p2 p2;

	memset(&st, 0, sizeof(st));

	n = scanf(
	    " Player 1 starting position: %d"
	    " Player 2 starting position: %d", &st.pos0, &st.pos1);
	if (n != 2)
		assert(!"bad input");

	st.pos0--; /* 0 indexed */
	st.pos1--;
	
	p1 = solve_p1(st);
	p2 = solve_p2(st);

	printf("21: %d %"PRIu64"\n", p1, MAX(p2.wins0, p2.wins1));
	return 0;
}
