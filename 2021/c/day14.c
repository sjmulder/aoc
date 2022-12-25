#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

/*
 * Data is kept as two maps: a count of letters and a count of
 * pairs. The counts of pairs are 'double buffered', that is to say
 * one represents the previous step, and one the next, so new pairs
 * aren't processed again in the same step.
 */

static uint64_t
score(uint64_t *ccounts)
{
	int c;
	uint64_t nmin=0,nmax=0;

	for (c=0; c<=26; c++) {
		if (!ccounts[c]) continue;
		if (!nmin || ccounts[c] < nmin) nmin = ccounts[c];
		if (!nmax || ccounts[c] > nmax) nmax = ccounts[c];
	}

	return nmax - nmin;
}

int
main()
{
	static uint8_t rules[26][26], line[50];
	static uint64_t pcounts[2][26][26];
	static uint64_t ccounts[256];
	int step, in,out, i;
	uint64_t count, p1=0,p2;
	uint8_t c1,c2,cnew;

	if (scanf(" %49s", line) != 1)
		assert(!"bad input");

	for (i=0; line[i]; i++) {
		line[i] -= 'A';
		ccounts[line[i]]++;
		if (i) pcounts[0][line[i-1]][line[i]]++;
	}

	while (scanf(" %c%c -> %c", &c1, &c2, &cnew) == 3)
		rules[c1-'A'][c2-'A'] = cnew-'A';

	for (step=0; step<40; step++) {
		in = step%2;
		out = (step+1)%2;
		memset(pcounts[out], 0, sizeof(pcounts[out]));

		for (c1=0; c1<26; c1++)
		for (c2=0; c2<26; c2++) {
			count = pcounts[in][c1][c2];
			cnew = rules[c1][c2];

			if (cnew) {
				ccounts[cnew] += count;
				pcounts[out][c1][cnew] += count;
				pcounts[out][cnew][c2] += count;
			} else
				pcounts[out][c1][c2] += count;
		}

		if (step == 9)
			p1 = score(ccounts);
	}

	p2 = score(ccounts);
	printf("14: %"PRIu64" %"PRIu64"\n", p1, p2);

	return 0;
}
