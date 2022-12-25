#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <err.h>

#define CAP 64

static int prefs[CAP][CAP];
static int count;

static void
parse(void)
{
	char first[16], who[16], whom[16], what[5];
	int i=1,j=0, n, pts;

	count = CAP;

	for (i=0; i<count; i++)
	for (j=0; j<count; j++) {
		if (i == j) continue;

		n = scanf(" %15s would %4s %d happiness units by "
		    "sitting next to %15s", who, what, &pts, whom);

		if (n != 4)                errx(1, "not enough data");
		if (!strcmp(what, "lose")) pts *= -1;

		if (i) ;
		else if (j==1)               memcpy(first, who, 16);
		else if (strcmp(first, who)) { count = j; i=1; j=0; }

		prefs[i][j] = pts;
	}
}

static int
solve(void)
{
	static int seats[CAP];
	int i,j, best=INT_MIN, score;

	while (1) {
skip:		for (i = count-1; seats[i] == count-1; )
			if (!i--)
				return best;
		seats[i]++;
		for (i = i+1; i<count; i++)
			seats[i] = 0;

		for (i=0; i<count; i++)
		for (j=0; j<i; j++)
			if (seats[i] == seats[j])
				goto skip;

		score = 0;
		for (i=0; i < count; i++) {
			score += prefs[seats[i]][seats[(i+1) % count]];
			score += prefs[seats[(i+1) % count]][seats[i]];
		}

		if (score > best)
			best = score;
	}
}

int
main()
{
	parse();

	printf("%d\n", solve()); count++;
	printf("%d\n", solve());

	return 0;
}
