#include <stdio.h>
#include <string.h>

#define SZ 10

static char grid[SZ][SZ+1];
static char flashed[SZ][SZ];

static void
flash(int r, int c)
{
	int r2,c2;

	if (flashed[r][c])
		return;

	flashed[r][c] = 1;

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

	fread(grid, 1, sizeof(grid), stdin);

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
			if (grid[r][c] > '9') {
				nflash++;
				grid[r][c] = '0';
			}

		if (i<100)
			p1 += nflash;

		memset(flashed, 0, sizeof(flashed));
	}

	printf("11: %d %d\n", p1, i);
	return 0;
}
