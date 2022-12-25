#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "compat/util.h"
#include "compat/stdint.h"
#include "compat/inttypes.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct range { int start, end; }; /* end non-inclusive */
struct sensor { int x,y, range; };

static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

static int
manhattan(int x0, int y0, int x1, int y1)
{
	return abs(x0-x1) + abs(y0-y1);
}

/*
 * Ranges must be sorted (by .start), non-overlapping.
 * The list is extended or modified to include the new range,
 * maintaining these invariants.
 */
static void
insert_range(struct range *ranges, size_t *len, struct range range)
{
	size_t i,j;

	/* find i: insertion/replacement point */
	for (i=0; i < *len; i++)
		if (ranges[i].end >= range.start)
			break;

	/* find j: first range to remain */ 
	for (j=i; j < *len; j++)
		if (ranges[j].start > range.end)
			break;

	/* extend new range to cover existing */
	if (i < *len) range.start = min(range.start, ranges[i].start);
	if (j > 0) range.end = max(range.end, ranges[j-1].end);

	/* replace i..j with 1 slot */
	memmove(&ranges[i+1], &ranges[j], sizeof(range) * (*len-j));
	*len = *len +1 - (j-i);

	ranges[i] = range;
}

static void UNUSED
print_ranges(struct range *ranges, size_t len)
{
	size_t i;

	printf("[%zu]", len);
	for (i=0; i<len; i++)
		printf(" %d..%d", ranges[i].start, ranges[i].end);
	putchar('\n');
}

#define INPUT_FMT \
" Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d"

#if 0	/* 1 for example input */
# define P1_Y	10
# define P2_SZ	20
#else
# define P1_Y	2000000
# define P2_SZ	4000000
#endif

int
main(int argc, char **argv)
{
	int beacons[64];	/* beacon x positions on test row */
	struct range seen[64];	/* scanned ranges on test row */
	struct sensor sensors[64], *sr;
	struct range range;
	size_t nbeacons=0, nseen=0, nsensors=0, i,j;
	int sx,sy, bx,by, x,y, dist, vert;
	int p1=0;
	int64_t p2=0;

	if (argc > 1)
		freopen(argv[1], "r", stdin);

	/*
	 * For part 1 we track what portions of the line are covered by
	 * sensors. Then we sum the lengths and subtract the beacons.
	 */

	while (scanf(INPUT_FMT, &sx,&sy, &bx,&by) == 4) {
		/* if on test row, store beacon if new */
		if (by == P1_Y) {
			for (i=0; i < nbeacons; i++)
				if (beacons[i] == bx)
					goto not_new;
			assert(nbeacons < LEN(beacons));
			beacons[nbeacons++] = bx;
		}
	not_new:

		/* mark beacon range on test row */
		dist = manhattan(sx,sy, bx,by);
		vert = abs(P1_Y-sy);
		range.start = (sx-dist) + vert;
		range.end   = (sx+dist) - vert +1; /* exclusive */

		/* store sensor */
		assert(nsensors < LEN(sensors));
		sensors[nsensors].x = sx;
		sensors[nsensors].y = sy;
		sensors[nsensors].range = dist+1;
		nsensors++;

		if (range.end > range.start) {
			assert(nseen+1 < LEN(seen));
			insert_range(seen, &nseen, range);
		}
	}

	for (i=0; i<nseen; i++) {
		p1 += seen[i].end - seen[i].start;

		for (j=0; j<nbeacons; j++)
			p1 -= beacons[j] >= seen[i].start &&
			      beacons[j] <  seen[i].end;
	}

	/*
	 * For part 2 we iterate over all the cells in range -but- with
	 * a major shortcut: when we find ourselves covered by a sensor,
	 * which is always except for the solution cell, we skip ahead
	 * to the end of the sensor's covered range on this line.
	 *
	 * We could also skip over rows in this manner but that involves
	 * some extra logic and this is fast enough for now.
	 */

	for (y=0; y<P2_SZ; y++)
	for (x=0; x<P2_SZ; x++) {
		for (i=0; i<nsensors; i++) {
			sr = &sensors[i];
			if (manhattan(x,y, sr->x,sr->y) < sr->range)
				break;
		}

		if (i == nsensors) {
			p2 = (int64_t)x*4000000 + y;
			goto have_p2;
		}

		x = sr->x + sr->range - abs(y - sr->y) -1;
	}
have_p2:

	printf("15: %d %" PRId64 "\n", p1, p2);
	return 0;

}
