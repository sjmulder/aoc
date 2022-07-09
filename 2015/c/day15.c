#include <stdio.h>
#include <limits.h>
#include <err.h>

#define MAX_INGS	10
#define NUM_ATTRS	5
#define NUM_SPOONS	100

static const char fmt[] =
//"%*s capacity %d, durability %d, flavor %d, texture %d, calories %d";
"%*s capacity %d, durability %d, flavor %d, texture %d, calories %d";

static int attrs[MAX_INGS][NUM_ATTRS];
static int counts[MAX_INGS];
static int nings;
static int p1=INT_MIN, p2=INT_MIN;

static void
recur(int nleft, int ing)
{
	int totals[NUM_ATTRS] = {};
	int score=1, i,j;

	if (ing == nings-1) {
		counts[ing] = nleft;

		//for (i=0; i < nings; i++)
		//	printf(" %d", counts[i]);
		//printf("\n");

		for (i=0; i<nings; i++)
		for (j=0; j<NUM_ATTRS; j++) /* excl. cals */
			totals[j] += attrs[i][j] * counts[i];

		for (i=0; i<NUM_ATTRS-1; i++)
			score *= totals[i] < 0 ? 0 : totals[i];

		if (score > p1)
			p1 = score;
		if (score > p2 && totals[NUM_ATTRS-1] == 500)
			p2 = score;
	} else {
		for (i=0; i <= nleft; i++) {
			counts[ing] = i;
			recur(nleft-i, ing+1);
		}
	}
}

int
main()
{
	while (scanf(fmt,
	    &attrs[nings][0],
	    &attrs[nings][1],
	    &attrs[nings][2],
	    &attrs[nings][3],
	    &attrs[nings][4]) == 5)
		if (++nings >= MAX_INGS)
			errx(1, "ingredients overflow");

	recur(NUM_SPOONS, 0);

	printf("15: %d %d\n", p1, p2);
	return 0;
}
