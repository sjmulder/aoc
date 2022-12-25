#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SZ	10
#define FLASHED	('9'+2)

static char grid[SZ][SZ+1];

static void
flash(int r, int c)
{
	int r2,c2;

	if (grid[r][c] == FLASHED)
		return;
	grid[r][c] = FLASHED;

	for (r2=r-1; r2<=r+1; r2++)
	for (c2=c-1; c2<=c+1; c2++) {
		if (r2<0 || r2>=SZ) continue;
		if (c2<0 || c2>=SZ) continue;
		if (r==r2 && c==c2) continue;

		if (grid[r2][c2] <= '9' && ++grid[r2][c2] > '9')
			flash(r2, c2);
	}
}

int
main()
{
	int i,r,c, p1=0, nflash=0;

	if (fread(grid, 1, sizeof(grid), stdin) != sizeof(grid))
		assert(!"failed to read input");

	for (i=0; nflash != SZ*SZ; i++) {
		nflash=0;

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			grid[r][c]++;

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			if (grid[r][c] > '9')
				flash(r, c);

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			if (grid[r][c] == FLASHED) {
				nflash++;
				grid[r][c] = '0';
			}

		if (i<100)
			p1 += nflash;
	}

	printf("11: %d %d\n", p1, i);
	return 0;
}
