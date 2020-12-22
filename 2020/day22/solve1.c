#include <stdio.h>
#include <string.h>
#include <assert.h>

#define NCARDS 52

int
main()
{
	int cards[2][NCARDS];
	int ncards[2] = {};
	char buf[16];
	int p=0,c, val, winner, score=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "Player %d", &p) == 1) {
			p--;
			assert(p>=0 && p<2);
		} else if (sscanf(buf, "%d", &val) == 1) {
			assert(ncards[p] < NCARDS);
			cards[p][ncards[p]++] = val;
		}
	};

	while (1) {
		winner = cards[0][0] < cards[1][0];

		assert(ncards[winner]+2 <= NCARDS);
		cards[winner][ncards[winner]++] = cards[ winner][0];
		cards[winner][ncards[winner]++] = cards[!winner][0];

		for (p=0; p<2; p++)
			memmove(cards[p], cards[p]+1,
			    sizeof(**cards) * --ncards[p]);

		if (!ncards[!winner])
			break;
	}

	for (c=0; c<ncards[winner]; c++)
		score += cards[winner][c] * (ncards[winner]-c);

	printf("%d\n", score);
}
