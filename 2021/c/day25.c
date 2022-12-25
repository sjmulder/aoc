#include <stdio.h>
#include <string.h>

int
main()
{
#define SZ 140

	static char grid[SZ][SZ];
	int w,h=0, x,y, step=0,dirty,wrap;

	while (h < SZ && scanf(" %139s", grid[h]) == 1) h++;
	w = strlen(grid[0]);

	do {
		step++; dirty=0;

		for (y=0; y<h; y++) {
			wrap = grid[y][w-1] == '>' &&
			       grid[y][0]   == '.';

			for (x=0; x<w-1; x++)
				if (grid[y][x]   == '>' &&
				    grid[y][x+1] == '.') {
					grid[y][x]   = '.';
					grid[y][x+1] = '>';
					dirty=1; x++;
				}

			if (wrap) {
				grid[y][w-1] = '.';
				grid[y][0]   = '>';
				dirty=1;
			}
		}

		for (x=0; x<w; x++) {
			wrap = grid[h-1][x] == 'v' &&
			       grid[0][x]   == '.';

			for (y=0; y<h-1; y++)
				if (grid[y][x]   == 'v' &&
				    grid[y+1][x] == '.') {
					grid[y][x]   = '.';
					grid[y+1][x] = 'v';
					dirty=1; y++;
				}

			if (wrap) {
				grid[h-1][x] = '.';
				grid[0][x]   = 'v';
				dirty=1;
			}
		}
	} while (dirty);

	printf("25: %d\n", step);
	return 0;
}
