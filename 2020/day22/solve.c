#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NCARDS 52
#define MAXROUNDS 1000

struct state {
	int cards[2][NCARDS];
	int ncards[2];
};

/* returns winner; score is optional */
static int
game(struct state *initst, int part, int *score)
{
	struct state st, st2;
	int p,c, round, win;

	st = *initst;

	for (round=0; ; round++) {
		/*
		 * Who needs hash tables when you can use
		 * /u/mendelmunkis' dirty tricks!
		 */
		if (round > MAXROUNDS)
			{ win=0; break; }

		if (part == 2 &&
		    st.cards[0][0] < st.ncards[0] &&
		    st.cards[1][0] < st.ncards[1]) {
			for (p=0; p<2; p++) {
				st2.ncards[p] = st.cards[p][0];
				memcpy(st2.cards[p], st.cards[p]+1,
				    sizeof(**st.cards) * st2.ncards[p]);
			}
			win = game(&st2, part, NULL);
		} else
			win = st.cards[0][0] < st.cards[1][0];

		assert(st.ncards[win]+2 <= NCARDS);
		st.cards[win][st.ncards[win]++] = st.cards[ win][0];
		st.cards[win][st.ncards[win]++] = st.cards[!win][0];

		for (p=0; p<2; p++)
			memmove(st.cards[p], st.cards[p]+1,
			    sizeof(**st.cards) * --st.ncards[p]);

		if (!st.ncards[!win])
			break;
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
	int p=0, val, pt1=0, pt2=0;

	while (fgets(buf, sizeof(buf), stdin))
		if (sscanf(buf, "Player %d", &p) == 1) {
			p--;
			assert(p>=0 && p<2);
		} else if (sscanf(buf, "%d", &val) == 1) {
			assert(st.ncards[p] < NCARDS);
			st.cards[p][st.ncards[p]++] = val;
		}

	game(&st, 1, &pt1);
	game(&st, 2, &pt2);
	printf("%d %d\n", pt1, pt2);
}
