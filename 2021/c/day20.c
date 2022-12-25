#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define SZ 210
#define X0 55
#define Y0 55

static char table[513];
static char grid[2][SZ][SZ];
static int gi; /* index of currently active grid */
static int xmin=X0, xmax=X0-1, ymin=Y0, ymax=Y0-1;

static void
step(void)
{
	int x,y;

	for (y=ymin-1; y<=ymax+1; y++)
	for (x=xmin-1; x<=xmax+1; x++)
		grid[!gi][y][x] = table[
		    grid[gi][y-1][x-1] << 8 |
		    grid[gi][y-1][x  ] << 7 |
		    grid[gi][y-1][x+1] << 6 |
		    grid[gi][y  ][x-1] << 5 |
		    grid[gi][y  ][x  ] << 4 |
		    grid[gi][y  ][x+1] << 3 |
		    grid[gi][y+1][x-1] << 2 |
		    grid[gi][y+1][x  ] << 1 |
		    grid[gi][y+1][x+1]];

	gi = !gi;
	xmin--; xmax++;
	ymin--; ymax++;

	assert(xmin > 2); assert(xmax < SZ-3);
	assert(ymin > 2); assert(ymax < SZ-3);

}

static int
count_on(void)
{
	int x,y, n=0;

	for (y=ymin; y<=ymax; y++)
	for (x=xmin; x<=xmax; x++)
		n += grid[gi][y][x];

	return n;
}

int
main()
{
	int len, i, p1,p2;

	if (scanf(" %512s", table) != 1)
		assert(!"bad input");

	len = strlen(table);
	for (i=0; i<len; i++)
		table[i] = table[i]=='#';

	/* account for empty cell rule */
	if (table[0])
		memset(grid[1], 1, sizeof(grid[1]));

	while (scanf(" %100s", &grid[0][ymax+1][X0]) == 1) {
		len = (int)strlen(&grid[0][ymax+1][X0]);
		xmax = MAX(xmax, X0+len-1);
		ymax++;
		for (i=0; i<len; i++)
			grid[0][ymax][X0+i] = grid[0][ymax][X0+i]=='#';
		assert(ymax < SZ-3);

	}

	for (i=0; i<2;  i++) { step(); } p1 = count_on();
	for (;    i<50; i++) { step(); } p2 = count_on();

	printf("20: %d %d\n", p1, p2);

	return 0;
}
