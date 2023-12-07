#include "common.h"

struct hand { char cards[6]; int bet, type; };

static struct hand hands[1024];
static size_t nhands;
static int cur_part=0;	/* global to get it to cmp_hand */

static int
cmp_int_desc(const void *a, const void *b)
{
	return *(int *)b - *(int *)a;
}

static int
cmp_hand(const void *a, const void *b)
{
	const struct hand *ha = a, *hb = b;
	const char *order;
	size_t i,j;

	if (ha->type != hb->type)
		return ha->type - hb->type;

	order = cur_part==0 ? "23456789TJQKA" : "J23456789TQKA";

	for (i=0; i < LEN(ha->cards); i++) {
		if (ha->cards[i] == hb->cards[i])
			continue;
		for (j=0; order[j]; j++)
			if (order[j] == ha->cards[i]) return -1; else
			if (order[j] == hb->cards[i]) return  1;
	}

	return 0;
}

static int
get_type(char *cards)
{
	static int counts[256];
	int nj=0;
	size_t i;

	memset(counts, 0, sizeof(counts));

	for (i=0; cards[i]; i++)
		counts[(int)cards[i]]++;
	if (cur_part) {
		nj = counts['J'];
		counts['J'] = 0;
	}

	/* sort only the few counts we care about */
	for (i=0; i<13; i++)
		counts[i] = counts[(int)"23456789TJQKA"[i]];

	qsort(counts, 13, sizeof(*counts), cmp_int_desc);

	if (counts[0]+nj == 5) return 6;
	if (counts[0]+nj == 4) return 5;
	if (counts[0]+nj == 3 && counts[1]    == 2) return 4;
	if (counts[0]    == 3 && counts[1]+nj == 2) return 4;
	if (counts[0]+nj == 3) return 3;
	if (counts[0]+nj == 2 && counts[1]    == 2) return 2;
	if (counts[0]    == 2 && counts[1]+nj == 2) return 2;
	if (counts[0]+nj == 2) return 1;
	return 0;
}

static int
solve(void)
{
	int score=0;
	size_t i;

	/* precompute, avoids repeatedly doing it in cmp_hand */
	for (i=0; i<nhands; i++)
		hands[i].type = get_type(hands[i].cards);

	qsort(hands, nhands, sizeof(*hands), cmp_hand);

	for (i=0; i<nhands; i++)
		score += hands[i].bet * (int)(i+1);

	return score;
}

int
main(int argc, char **argv)
{
	int p1,p2, nt;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (1) {
		assert(nhands < LEN(hands));
		nt = fscanf(stdin, " %5s %d", hands[nhands].cards,
		    &hands[nhands].bet);
		if (nt != 2) break;
		nhands++;
	}

	cur_part=0; p1 = solve();
	cur_part=1; p2 = solve();

	printf("%d %d\n", p1, p2);
	return 0;
}
