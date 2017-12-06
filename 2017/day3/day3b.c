#define SZ  63
#define MID (SZ/2)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
	int grid[64][64], edge, x=MID, y=MID, i, target;

	if (argc < 2) {
		fprintf(stderr, "no number given\n");
		return 1;
	}

	memset(grid, 0, sizeof(grid));
	grid[x][y] = 1;
	target = (int)strtol(argv[1], NULL, 10) - 1;

	for (edge = 0; edge/2 < SZ; edge++) {
		for (i = 0; i <= edge/2; i++) {
			grid[x][y] =
				grid[x-1][y-1] + grid[x][y-1] + grid[x+1][y-1] +
				grid[x-1][y]   + grid[x][y ]  + grid[x+1][y]   +
				grid[x-1][y+1] + grid[x][y+1] + grid[x+1][y+1];

			if (grid[x][y] > target) {
				printf("%d\n", grid[x][y]);
				return 0;
			}

			switch (edge % 4) {
				case 0: x++; break;
				case 1: y--; break;
				case 2: x--; break;
				case 3: y++; break;
			}
		}
	}

	puts("not found");
	return 0;
}
