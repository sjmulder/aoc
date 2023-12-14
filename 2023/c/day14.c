#include "common.h"

static char g[101][102];
static int w,h;

static int
hash_grid(void)
{
	int acc=0, x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		acc = acc*31 + g[y][x];

	return acc;
}

static int
score_grid(void)
{
	int acc=0, x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x]=='O')
			acc += h-y;

	return acc;
}

static void
shift_north(void)
{
	int x,i,j;

	for (x=0; x<w; x++)
	for (i=j=0; i<h; i++) {
		for (j = MAX(i+1,j); j<h && g[j][x]=='.'; j++)
			;
		if (g[i][x]=='.' && g[j][x]=='O') {
			g[i][x] = 'O';
			g[j][x] = '.';
		}
	}
}

static void
shift_all(void)
{
	int x,y,i,j;

	shift_north();

	for (y=0; y<h; y++)
	for (i=j=0; i<w; i++) {
		for (j = MAX(i+1,j); j<w && g[y][j]=='.'; j++)
			;
		if (g[y][i]=='.' && g[y][j]=='O') {
			g[y][i] = 'O';
			g[y][j] = '.';
		}
	}

	for (x=0; x<w; x++)
	for (i=j=h-1; i>=0; i--) {
		for (j = MIN(i-1,j); j>=0 && g[j][x]=='.'; j--)
			;
		if (g[i][x]=='.' && g[j][x]=='O') {
			g[i][x] = 'O';
			g[j][x] = '.';
		}
	}

	for (y=0; y<h; y++)
	for (i=j=w-1; i>=0; i--) {
		for (j = MIN(i-1,j); j>=0 && g[y][j]=='.'; j--)
			;
		if (g[y][i]=='.' && g[y][j]=='O') {
			g[y][i] = 'O';
			g[y][j] = '.';
		}
	}
}

int
main(int argc, char **argv)
{
	static int hist[256];	/* hashes */
	int p1=0,p2=0, nhist=0,period=0, nleft, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (h=0; fgets(g[h], LEN(*g), stdin); h++)
		assert(h+1 < (int)LEN(g));
	
	assert(h>0); w = strlen(g[0])-1;
	assert(w>0);

	shift_north();
	p1 = score_grid();

	for (nleft = 1*1000*1000*1000; nleft; nleft--) {
		shift_all();

		if (period)
			continue;

		assert(nhist < (int)LEN(hist));
		hist[nhist++] = hash_grid();

		for (i=0; i<nhist-1; i++)
			if (hist[i] == hist[nhist-1]) {
				period = nhist-1 - i;
				nleft = nleft % period;
				break;
			}
	}

	p2 = score_grid();

	printf("14: %d %d\n", p1, p2);
	return 0;
}
