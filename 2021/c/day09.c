#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define SZ 100

static char grid[SZ][SZ+3], visited[SZ][SZ];
static int counts[SZ*SZ/2];
static int w,h, ngroups;

static int
cmp_rint(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}

static void
flood(int r, int c, int group)
{
	if (visited[r][c] || grid[r][c] == '9')
		return;

	visited[r][c] = 1;
	counts[group]++;

	if (r) flood(r-1, c, group);
	if (c) flood(r, c-1, group);
	if (r<h-1) flood(r+1, c, group);
	if (c<w-1) flood(r, c+1, group);
}

int
main()
{
	int r,c, p1=0,p2;

	while (scanf(" %100s", grid[h]) == 1)
		{ h++; assert(h <= SZ); }
	w = (int)strlen(grid[0]);

	for (r=0; r<h; r++)
	for (c=0; c<w; c++) {
		if ((!r || grid[r][c] < grid[r-1][c]) &&
		    (!c || grid[r][c] < grid[r][c-1]) &&
		    (r>=h-1 || grid[r][c] < grid[r+1][c]) &&
		    (c>=w-1 || grid[r][c] < grid[r][c+1]))
			p1 += 1 + grid[r][c]-'0';
		if (!visited[r][c] && grid[r][c] != '9')
			flood(r, c, ngroups++);
	}

	qsort(counts, ngroups, sizeof(*counts), cmp_rint);
	p2 = counts[0] * counts[1] * counts[2];

	printf("09: %d %d\n", p1, p2);
	return 0;
}
