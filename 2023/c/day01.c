#include "common.h"

int
main(int argc, char **argv)
{
	/*
	 * O(n) implementation, that is with no lookahead or
	 * backtracking. That's trivial for part 1 but for part 2 we
	 * need to match against words.
         *
	 * We do that by keeping, for each word, a running tally of how
	 * many letters we've matched so far. E.g. if we're 2 chars into
	 * a match for "one" and 0 chars into a match for "eight", an
	 * 'e' would advance the match for both (to 3 and 1
	 * respectively).
	 */

	static const char names[][8] = {"zero", "one", "two", "three",
	    "four", "five", "six", "seven", "eight", "nine"};
	long p1=0,p2=0;	/* long for 16-bit platforms */
	int p1_first=-1, p1_last=-1;
	int p2_first=-1, p2_last=-1, i,c;
	int nmatched[10] = {0};

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while ((c = getchar()) != EOF)
		if (c == '\n') {
			p1 += p1_first*10 + p1_last;
			p2 += p2_first*10 + p2_last;
			p1_first = p1_last = p2_first = p2_last = -1;
			memset(nmatched, 0, sizeof(nmatched));
		} else if (c >= '0' && c <= '9') {
			if (p1_first == -1) p1_first = c-'0';
			if (p2_first == -1) p2_first = c-'0';
			p1_last = p2_last = c-'0';
			memset(nmatched, 0, sizeof(nmatched));
		} else for (i=0; i<10; i++)
			/* advance or reset no. matched digit chars */
			if (c != names[i][nmatched[i]++])
				nmatched[i] = c == names[i][0];
			/* matched to end? */
			else if (!names[i][nmatched[i]]) {
				if (p2_first == -1) p2_first = i;
				p2_last = i;
				nmatched[i] = 0;
			}

	printf("01: %ld %ld\n", p1, p2);
	return 0;
}
