#include <stdio.h>
#include <string.h>
#include <err.h>

#define SZ	102

static char grid[SZ][SZ], grid_orig[SZ][SZ];

static void
step(int is_p2)
{
	int x,y, dx,dy;

	static char nadj[SZ][SZ];

	memset(nadj, 0, sizeof(nadj));

	if (is_p2) {
		grid[1][1] = '#';
		grid[1][SZ-2] = '#';
		grid[SZ-2][1] = '#';
		grid[SZ-2][SZ-2] = '#';
	}

	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
	for (dy=-1; dy<2; dy++)
	for (dx=-1; dx<2; dx++)
		nadj[y][x] += grid[y+dy][x+dx] == '#';

	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		grid[y][x] =
		    (is_p2 && (x == 1 || x == SZ-2) &&
			      (y == 1 || y == SZ-2)) ||
		     nadj[y][x] == 3 ||
		    (nadj[y][x] == 4 && grid[y][x] == '#') ? '#' : '.';
}

static int
count_on(void)
{
	int x,y, count=0;

	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		count += grid[y][x] == '#';

	return count;
}

int
main()
{
	int y=1, i, p1, p2;

	/* we allow the read to overflow 1 char into the next row (the \0) */
	while (fgets(grid_orig[y]+1, SZ, stdin))
		if (++y >= SZ)
			errx(1, "too many rows");

	memcpy(grid, grid_orig, sizeof(grid));
	for (i=0; i<100; i++)
		step(0);
	p1 = count_on();

	memcpy(grid, grid_orig, sizeof(grid));
	for (i=0; i<100; i++)
		step(1);
	p2 = count_on();

	printf("18: %d %d\n", p1, p2);
	return 0;
}
