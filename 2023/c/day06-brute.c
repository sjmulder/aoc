#include "common.h"

static void
parse_line(int64_t nums[4], int64_t *bignum)
{
	static char line[64];
	char *s;

	fgets(line, sizeof(line), stdin);

	sscanf(line, "%*s %"SCNi64" %"SCNi64" %"SCNi64" %"SCNi64,
	    &nums[0], &nums[1], &nums[2], &nums[3]);

	for (*bignum = 0, s = line; *s; s++)
		if (isdigit(*s))
			*bignum = *bignum * 10 + (*s-'0');
}

static int64_t
solve(int64_t *times, int64_t *recs, size_t n)
{
	size_t i;
	int64_t acc=1, best,nbetter, tp,d;

	for (i=0; i<n && times[i]; i++) {
		best = nbetter = 0;

		for (tp=1; tp < times[i]; tp++) {
			d = tp * (times[i]-tp);
			if (d > recs[i])
				nbetter++;
			else if (d < best)
				break;
			best = d;
		}

		acc *= nbetter;
	}

	return acc;
}

int
main(int argc, char **argv)
{
	int64_t times[4], recs[4], bigtime, bigrec, p1,p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	parse_line(times, &bigtime);
	parse_line(recs, &bigrec);

	p1 = solve(times, recs, 4);
	p2 = solve(&bigtime, &bigrec, 1);

	printf("06: %"PRIi64" %"PRIi64"\n", p1, p2);
	return 0;
}
