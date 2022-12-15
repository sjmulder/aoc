#include <stdio.h>
#include <string.h>

struct range { int start, end; }; /* end non-inclusive */

static int min(int a, int b) { return a<b ? a : b; }
static int max(int a, int b) { return a>b ? a : b; }

/*
 * Ranges must be sorted (by .start), non-overlapping.
 * The list is extended or modified to include the new range,
 * maintaining these invariants.
 */
static void
insert_range(struct range *ranges, size_t *len, struct range range)
{
	size_t i,j;

	/*
	 * Example scenario 1:
	 *  ### ###   ####  ###   ###
	 *      --###########-- 
	 *      ^                 ^
	 *      i                 j
	 *
	 * Example scenario 2:
	 *  ### ###   ####  ###   ###
	 *          ############
	 *      ^                 ^
	 *      i                 j
	 */

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

static void
dump_ranges(struct range *ranges, size_t len)
{
	size_t i;

	printf("[%zu]", len);
	for (i=0; i<len; i++)
		printf(" %d..%d", ranges[i].start, ranges[i].end);
	putchar('\n');
}

int
main()
{
	struct range ranges[10]={}, range;
	size_t nranges=0;

	dump_ranges(ranges, nranges);

	range.start = 5;
	range.end = 10;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* adjecent (before) */
	range.start = 0;
	range.end = 5;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* adjecent (after) */
	range.start = 10;
	range.end = 15;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* after */
	range.start = 25;
	range.end = 35;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* overlap (front) */
	range.start = -5;
	range.end = 5;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* overlap (end) */
	range.start = 10;
	range.end = 20;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	/* overlap (both) */
	range.start = 15;
	range.end = 30;
	insert_range(ranges, &nranges, range);
	dump_ranges(ranges, nranges);

	return 0;

}
