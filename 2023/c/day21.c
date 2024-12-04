#include "common.h"

#define GSZ 135

static char map[GSZ][GSZ];	/* all reachable replaced by \0 */
static int nobs[2][2];		/* [in/out diamond][even/odd t] */
static int sz;

/*
 * We call the big diamond in the input (covered by part 1) the 'inside 
 * diamond' and the diamond found by combining the corner pieces on the
 * 'outside diamond'. For both we found the number of unreachable tiles
 * (obstacles, 'nobs') for the even and odd time steps.
 *
 * The total number of tiles covered in t is (t+1)^2 - accounting for
 * the checkerboard reachability pattern. With the part 1 and 2
 * questions, this exactly covers a certain square amount of diamonds
 * (ndia*ndia).
 *
 * Finding out which types of diamonds and in what state (odd/even) is
 * the tricky bit, e.g. the state alternates between the repeating grids
 * because the grid size is uneven.
 */
static uint64_t
find_reach(uint64_t t)
{
	uint64_t ndia;
	int odd;

	odd = (int)(t&1);
	ndia = t/sz*2 +1;

	return (t+1)*(t+1) -
	    (ndia/2 +!odd) * (ndia/2 +!odd) * nobs[0][0] -
	    (ndia/2 + odd) * (ndia/2 + odd) * nobs[0][1] -
	    ndia*ndia/4 * nobs[1][0] -
	    ndia*ndia/4 * nobs[1][1];
}

int
main(int argc, char **argv)
{
	int dirty=0, odd,out, x,y;
	uint64_t p1, p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	/* parse map */
	for (y=0; fgets(map[y], sizeof(map[y]), stdin); y++)
		assert(y+1 < GSZ);

	sz = y;
	assert(sz >= 0);
	assert(map[0][sz] == '\n');

	/* clear (\0) all reachable tiles */
	map[0][0] = 0;

	for (dirty=1; dirty; )
	for (y=0, dirty=0; y<sz; y++)
	for (x=0; x<sz; x++)
		if (map[y][x] == '.' && (
		    (x>0    && !map[y][x-1]) ||
		    (x<sz-1 && !map[y][x+1]) ||
		    (y>0    && !map[y-1][x]) ||
		    (y<sz-1 && !map[y+1][x])))
			{ dirty = 1; map[y][x] = 0; }

	/* count obstacles */
	for (y=0; y<sz; y++)
	for (x=0; x<sz; x++)
		if (map[y][x]) {
			out = abs(sz/2-x) + abs(sz/2-y) > sz/2;
			odd = (x+y)&1;
			nobs[out][odd]++;
		}

	p1 = find_reach(64);
	p2 = find_reach(26501365);

	printf("21: %"PRIu64" %"PRIu64"\n", p1, p2);
		return 0;
}
