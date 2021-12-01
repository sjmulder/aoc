#include <stdio.h>
#include <string.h>
#include <assert.h>

#define CAP	128
#define NTRIALS	100

static char g[CAP][CAP], nadj[CAP][CAP], *adjp[CAP][CAP][8];
static int w,h;

static void
precomp(int hitdepth)
{
	int x,y, dir, dx,dy, x2,y2, i;

	memset(adjp, 0, sizeof(adjp));

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		dir = 0;

		for (dx=-1; dx<2; dx++)
		for (dy=-1; dy<2; dy++) {
			if (!dx && !dy)
				continue;
			for (i=0, y2=y, x2=x; i<hitdepth; i++) {
				if ((y2 += dy) < 0 || y2 >= h) break;
				if ((x2 += dx) < 0 || x2 >= w) break;
				if (g[y2][x2] != '.') {
					adjp[y][x][dir] = &g[y2][x2];
					break;
				}
			}
			dir++;
		}
	}
}

static int
step(int adjrule)
{
	int x,y,dir, nchange=0;

	memset(nadj, 0, sizeof(nadj));

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
	for (dir=0; dir<8; dir++)
		if (adjp[y][x][dir])
			nadj[y][x] += *adjp[y][x][dir] == '#';

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x] == 'L' && !nadj[y][x])
			{ nchange++; g[y][x] = '#'; }
		else if (g[y][x] == '#' && nadj[y][x] >= adjrule)
			{ nchange++; g[y][x] = 'L'; }

	return nchange;
}

static int
run(int hitdepth, int adjrule)
{
	int x,y, nocc=0;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x] == '#')
			g[y][x] = 'L';

	precomp(hitdepth);

	while (step(adjrule))
		;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		nocc += g[y][x] == '#';

	return nocc;
}

int
main(int argc, char **argv)
{
	FILE *f;
	char *c;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	for (h=0; h<CAP; h++) {
		if (!fgets(g[h], CAP, f))
			break;
		c = strchr(g[h], '\n');
		assert(c && "input too wide");
		if (h==1)
			w = (int)(c-g[h]);
	}

	printf("%d ",  run(1, 4));
	printf("%d\n", run(CAP, 5));

	//getchar();
	return 0;
}
