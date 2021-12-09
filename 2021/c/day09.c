#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define SZ 100

static uint8_t grid[SZ][SZ];
static int groups[SZ][SZ], counts[SZ*SZ/2];
static int w,h, ngroups;

static int
cmp_rint(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}

static void
flood(int r, int c, int group)
{
	if (groups[r][c] || grid[r][c] == 9)
		return;

	groups[r][c] = group;
	counts[group-1]++;

	if (r) flood(r-1, c, group);
	if (c) flood(r, c-1, group);
	if (r<h-1) flood(r+1, c, group);
	if (c<w-1) flood(r, c+1, group);
}

int
main()
{
	char buf[SZ+3], *lf;
	int r,c, p1=0,p2;

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((lf = strchr(buf, '\n'))) *lf = '\0';
		if (!h++) w = strlen(buf);
		assert(h <= SZ);
		assert(w <= SZ);
		for (c=0; c<w; c++) grid[h-1][c] = buf[c]-'0';
	}

	for (r=0; r<h; r++)
	for (c=0; c<w; c++) {
		p1 += (!r || grid[r][c] < grid[r-1][c]) *
		      (!c || grid[r][c] < grid[r][c-1]) *
		      (r>=h-1 || grid[r][c] < grid[r+1][c]) *
		      (c>=w-1 || grid[r][c] < grid[r][c+1]) *
		      (1 + grid[r][c]);
		if (!groups[r][c] && grid[r][c] != 9)
			flood(r, c, ++ngroups);
	}

	qsort(counts, ngroups, sizeof(*counts), cmp_rint);
	p2 = counts[0] * counts[1] * counts[2];

	printf("09: %d %d\n", p1, p2);
	return 0;
}
