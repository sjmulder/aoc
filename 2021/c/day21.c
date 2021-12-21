#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

struct state { int pos[2], sc[2]; };
struct p2 { uint64_t wins[2]; };

static struct state
apply_throw(struct state st, int p, int throw)
{
	st.pos[p] = (st.pos[p] + throw) % 10;
	st.sc[p] += st.pos[p]+1;
	return st;
}

static void
res_add(struct p2 *res, struct p2 other, int count)
{
	res->wins[0] += other.wins[0] * count;
	res->wins[1] += other.wins[1] * count;
}

static int
solve_p1(struct state st)
{
	int nthrows=0, i,p, throw;

	for (;;)
	for (p=0; p<2; p++) {
		for (throw=0, i=0; i<3; i++)
			throw += nthrows++ % 100 +1;

		st = apply_throw(st, p, throw);

		if (st.sc[p] >= 1000)
			return MIN(st.sc[0], st.sc[1]) * nthrows;
	}
}

static struct p2
solve_p2(struct state s, int p)
{
	static char incache[10][20][10][20][2];
	static struct p2 cache[10][20][10][20][2];
	struct p2 res={};

	printf("solve_p2(%d,%d, %d,%d, %d)",
	    s.pos[0],s.sc[0], s.pos[1],s.sc[1], p);

	if (s.sc[!p] >= 21) {
		printf(" win\n");
		res.wins[!p] = 1;
		return res;
	} else if (incache[s.pos[0]][s.sc[0]][s.pos[1]][s.sc[1]][p]) {
		res = cache[s.pos[0]][s.sc[0]][s.pos[1]][s.sc[1]][p];
		printf(" in cache (%"PRIu64", %"PRIu64")\n",
		    res.wins[0], res.wins[1]);
		return res;
	} else {
		printf(" recur\n");
		res_add(&res, solve_p2(apply_throw(s,p, 3), !p), 1);
		res_add(&res, solve_p2(apply_throw(s,p, 4), !p), 3);
		res_add(&res, solve_p2(apply_throw(s,p, 5), !p), 6);
		res_add(&res, solve_p2(apply_throw(s,p, 6), !p), 7);
		res_add(&res, solve_p2(apply_throw(s,p, 7), !p), 6);
		res_add(&res, solve_p2(apply_throw(s,p, 8), !p), 3);
		res_add(&res, solve_p2(apply_throw(s,p, 9), !p), 1);

		incache[s.pos[0]][s.sc[0]][s.pos[1]][s.sc[1]][p] = 1;
		cache[s.pos[0]][s.sc[0]][s.pos[1]][s.sc[1]][p] = res;

		return res;
	}
}

int
main(void)
{
	struct state st={};
	int p1;
	struct p2 p2;

	scanf(" Player 1 starting position: %d"
	      " Player 2 starting position: %d", st.pos, st.pos+1);
	
	st.pos[0]--; /* 0 indexed */
	st.pos[1]--;
	
	p1 = solve_p1(st);
	p2 = solve_p2(st, 0);

	printf("21: %d %"PRIu64"\n", p1, MAX(p2.wins[0], p2.wins[1]));
	return 0;
}
