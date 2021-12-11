#include <stdio.h>
#include <string.h>

#define SZ 10

static char grid[SZ][SZ+1];
static char flashed[SZ][SZ];

static void
dump()
{
	int r;

	for (r=0; r<SZ; r++)
		printf("%.*s\n", SZ, grid[r]);
}

static void
flash(int r, int c)
{
	int r2,c2;

	if (flashed[r][c])
		return;

	//printf("flash(%d, %d) grid=%c\n", r, c, grid[r][c]);
	flashed[r][c] = 1;

	//printf(" (%d,%d) flashes\n", r,c);
	//dump();
	//putchar('\n');

	for (r2=r-1; r2<=r+1; r2++)
	for (c2=c-1; c2<=c+1; c2++)
		if (r2>=0 && r2<SZ && c2>=0 && c2<SZ &&
		    (r!=r2 || c!=c2) && grid[r2][c2] <= '9')
			if (++grid[r2][c2] > '9')
				flash(r2, c2);
}

int
main()
{
	int i,r,c, p1=0,p2, all=0;

	fread(grid, 1, sizeof(grid), stdin);
	//dump();
	//putchar('\n');

	for (i=0; i<100 || !all; i++) {
		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			grid[r][c]++;

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++) {
			if (grid[r][c] > '9')
				flash(r, c);
			//printf("__\n");
		}

		//dump();

		all=1;
		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			if (grid[r][c] <= '9')
				all = 0;
			else {
				if (i<100) p1++;
				grid[r][c] = '0';
			}

		memset(flashed, 0, sizeof(flashed));
		//printf(" flashes: %d\n\n", nflashes);
	}

	printf("11: %d %d\n", p1, i);
	return 0;
}
