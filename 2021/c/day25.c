#include <stdio.h>
#include <string.h>

int
main()
{
#define SZ 140

	static char grid[2][SZ][SZ];
	int w,h=0, x,y, step=0,dirty;

	while (h < SZ && scanf(" %139s", grid[0][h]) == 1) h++;
	w = strlen(grid[0][0]);

	do {
		step++; dirty=0;
		memcpy(grid[1], grid[0], sizeof(*grid));

		for (y=0; y<h; y++)
		for (x=1; x<=w; x++)
			if (grid[0][y][x-1] == '>' &&
			    grid[0][y][x%w] == '.') {
				grid[1][y][x-1] = '.';
				grid[1][y][x%w] = '>';
				dirty = 1;
			}

		memcpy(grid[0], grid[1], sizeof(*grid));

		for (y=1; y<=h; y++)
		for (x=0; x<w; x++)
			if (grid[1][y-1][x] == 'v' &&
			    grid[1][y%h][x] == '.') {
				grid[0][y-1][x] = '.';
				grid[0][y%h][x] = 'v';
				dirty = 1;
			}
	} while (dirty);

	printf("25: %d\n", step);
}
