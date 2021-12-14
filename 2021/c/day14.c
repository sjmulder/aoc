#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

/*
 * Data is kept as two maps: a count of letters and a count of
 * pairs. The counts of pairs are 'double buffered', that is to say
 * one represents the previous step, and one the next, so new pairs
 * aren't processed again in the same step.
 *
 * The maps are represented as flat arrays indexed by IDX(), which
 * generates a 10-bit index (range 0-1024) from the lower 5 bits
 * of the two letters, discarding bit 5 (uppercase) and 4 (control).
 */

#define IDX(c1,c2)	((((int)(c1)&31) <<5) | ((int)(c2)&31))
#define NIDX		(1<<10)

static uint64_t
score(uint64_t *ccounts)
{
	int c;
	uint64_t nmin=0,nmax=0;

	for (c='A'; c<='Z'; c++) {
		if (!ccounts[c]) continue;
		if (!nmin || ccounts[c] < nmin) nmin = ccounts[c];
		if (!nmax || ccounts[c] > nmax) nmax = ccounts[c];
	}

	return nmax - nmin;
}

int
main()
{
	static char table[NIDX], line[50];
	static uint64_t pcounts[2][NIDX];
	static uint64_t ccounts[NIDX];
	int step, i;
	uint64_t *pcounts_in, *pcounts_out;
	uint64_t count, p1=0,p2;
	char c1,c2,cnew;

	scanf(" %49s", line);
	for (i=0; line[i]; i++) {
		ccounts[(int)line[i]]++;
		if (i) pcounts[0][IDX(line[i-1], line[i])]++;
	}

	while (scanf(" %c%c -> %c", &c1, &c2, &cnew) == 3)
		table[IDX(c1, c2)] = cnew;

	for (step=0; step<40; step++) {
		pcounts_in = pcounts[step%2];
		pcounts_out = pcounts[(step+1)%2];
		memset(pcounts_out, 0, sizeof(*pcounts));

		for (c1='A'; c1<='Z'; c1++)
		for (c2='A'; c2<='Z'; c2++) {
			count = pcounts_in[IDX(c1,c2)];
			cnew = table[IDX(c1,c2)];

			if (cnew) {
				ccounts[(int)cnew] += count;
				pcounts_out[IDX(c1,cnew)] += count;
				pcounts_out[IDX(cnew,c2)] += count;
			} else
				pcounts_out[IDX(c1,c2)] += count;
		}

		if (step == 9)
			p1 = score(ccounts);
	}

	p2 = score(ccounts);
	printf("14: %"PRIu64" %"PRIu64"\n", p1, p2);

	return 0;
}
