#include <stdio.h>
#include <limits.h>
#include <err.h>

#define CAP	50

static int sizes[CAP];
static int num, min_conts=INT_MAX, min_conts_count=0;

static int
count_combs(int idx, int liters, int nconts)
{
	if (liters == 0) {
		if (nconts == min_conts)
			min_conts_count++;
		else if (nconts < min_conts) {
			min_conts_count = 1;
			min_conts = nconts;
		}
		return 1;
	}
	if (idx >= num)
		return 0;

	if (sizes[idx] <= liters)
		return count_combs(idx+1, liters-sizes[idx], nconts+1) +
		       count_combs(idx+1, liters, nconts);
	else
		return count_combs(idx+1, liters, nconts);
}

int
main()
{
	int ncombs;

	while (scanf(" %d", &sizes[num]) == 1)
		if (++num >= CAP)
			errx(1, "sizes overflow");

	ncombs = count_combs(0, 150, 0);

	printf("17: %d %d\n", ncombs, min_conts_count);
	return 0;
}
