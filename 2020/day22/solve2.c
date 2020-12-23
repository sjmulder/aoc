#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NCARDS 52
#define NHIST (8*1024)

struct state {
	int cards[2][NCARDS];
	int ncards[2];
};

/* returns winner; score is optional */
static int
game(struct state *initst, int *score)
{
	struct state *hist;
	struct state st, st2;
	int p,c,i, nhist=0, win, bailout=0;

	hist = calloc(NHIST, sizeof(*hist));
	assert(hist);
	
	st = *initst;

	while (1) {
		for (p=0; p<2; p++)
			if (st.cards[p][0] >= st.ncards[p])
				break;

		if (p==2) {
			for (p=0; p<2; p++) {
				st2.ncards[p] = st.cards[p][0];
				memcpy(st2.cards[p], st.cards[p]+1,
				    sizeof(**st.cards) * st2.ncards[p]);
			}
			win = game(&st2, NULL);
		} else
			win = st.cards[0][0] < st.cards[1][0];

		for (i=0; i<nhist-1; i++)
			if (!memcmp(&st, &hist[i], sizeof(st)))
				{ win = 0; bailout = 1; break; }

		assert(st.ncards[win]+2 <= NCARDS);
		st.cards[win][st.ncards[win]++] = st.cards[ win][0];
		st.cards[win][st.ncards[win]++] = st.cards[!win][0];

		for (p=0; p<2; p++)
			memmove(st.cards[p], st.cards[p]+1,
			    sizeof(**st.cards) * --st.ncards[p]);

		if (bailout || !st.ncards[!win])
			break;

		assert(nhist < NHIST);
		hist[nhist++] = st;
	}

	if (score)
		for (c=0; c<st.ncards[win]; c++)
			*score += st.cards[win][c] * (st.ncards[win]-c);
	
	return win;
}

int
main()
{
	struct state st = {};
	char buf[16];
	int p=0, val, score=0;

	while (fgets(buf, sizeof(buf), stdin))
		if (sscanf(buf, "Player %d", &p) == 1) {
			p--;
			assert(p>=0 && p<2);
		} else if (sscanf(buf, "%d", &val) == 1) {
			assert(st.ncards[p] < NCARDS);
			st.cards[p][st.ncards[p]++] = val;
		}

	game(&st, &score);
	printf("%d\n", score);
}
