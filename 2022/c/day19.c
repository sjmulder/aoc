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

//static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

static void
read_input(void)
{
	static char buf[196];
	char *rest, *s;
	int rres, cres, cost;

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		assert(nbp < LEN(bps));
		nbp++;

		rest = strchr(rest, ':'); assert(rest);
		rest++; assert(rest);

		while ((s = strsep(&rest, "."))) {
			if (!*s || *s == '\n' || *s == '\r')
				break;

			rres =
			    !strncmp(s, " Each ge", 8) ? 0 :
			    !strncmp(s, " Each ob", 8) ? 1 :
			    !strncmp(s, " Each cl", 8) ? 2 :
			    !strncmp(s, " Each or", 8) ? 3 : -1;
			assert(rres != -1);
			assert(rres < NRES);

			while (1) {
				while (*s && !isdigit(*s))
					s++;
				if (!*s)
					break;

				cost = (int)strtol(s, &s, 10);
				assert(cost >= 0);

				cres =
				    !strncmp(s, " ge", 3) ? 0 :
				    !strncmp(s, " ob", 3) ? 1 :
				    !strncmp(s, " cl", 3) ? 2 :
				    !strncmp(s, " or", 3) ? 3 : -1;
				assert(cres != -1);
				assert(cres < NRES);

				bps[nbp-1].price[rres][cres] = cost;
				s += 4;
			}
		}
	}
}

static int
recur(struct st st)
{
	static int depth;
	struct st st2;
	int i,j, tbuy, best=0;

	if (st.t <= 0)
		return st.res[GEODE];

	/* option 1: wait it out if we have GEODE income */
	if (st.robos[GEODE]) {
		best = max(best, st.res[GEODE] + st.t*st.robos[GEODE]);
	}

	/* option 2[i]: save up for new robot of type i */
	for (i=0; i<NRES; i++) {
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

		best = max(best, st.res[GEODE] + st.t*st.robos[GEODE]);

		/* skip ahead and buy */
		st2 = st;
		st2.t -= tbuy;
		for (j=0; j<NRES; j++) {
			st2.res[j] += st2.robos[j] * tbuy;
			st2.res[j] -= st2.bp->price[i][j];
		}
		st2.robos[i]++;

		depth++;
		best = max(best, recur(st2));
		depth--;
	nobuy: ;
	}

	return best;
}

int
main()
{
	struct st st;
	int i, val, p1=0, p2=0;

	read_input();

	memset(&st, 0, sizeof(st));
	st.robos[ORE] = 1;

	for (i=0; i < (int)nbp; i++) {
		st.t = 24;
		st.bp = &bps[i];
		p1 += (i+1) * (val = recur(st));
		//printf(" p1 bp %d: val=%d q=%d\n", i, val, (i+1)*val);
	}
#if 0
	for (i=0; i < min((int)nbp, 3); i++) {
		st.t = 32;
		st.bp = &bps[i];
		p2 += (i+1) * (val = recur(st));
		//printf(" p2 bp %d: val=%d q=%d\n", i, val, (i+1)*val);
	}
#endif

	printf("19: %d %d\n", p1, p2);
	return 0;
}
