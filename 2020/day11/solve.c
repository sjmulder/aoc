#include <stdio.h>
#include <string.h>
#include <err.h>

#define CAP	128
#define VERBOSE	0

static char g[CAP][CAP], nadj[CAP][CAP];
static int w,h;

static void
dump(void)
{
#if VERBOSE
	int x,y;

	printf("  %d x %d\n", w, h);

	for (y=0; y<h; y++) {
		for (x=0; x<w; x++)
			printf("%d", nadj[y][x]);
		printf(" %.*s\n", w, g[y]);
	}

	putchar('\n');
#endif
}

static void
reset(void)
{
	int x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x] == '#')
			g[y][x] = 'L';
}

static int
step(int hitdepth, int adjrule)
{
	int i, x,y, dx,dy, x2,y2, nchange=0;

	memset(nadj, 0, sizeof(nadj));

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
	for (dx=-1; dx<2; dx++)
	for (dy=-1; dy<2; dy++)
		if (dx || dy)
			for (i=0, y2=y, x2=x; i<hitdepth; i++) {
				if ((y2 += dy) < 0 || y2 >= h) break;
				if ((x2 += dx) < 0 || x2 >= w) break;
				if (g[y2][x2] == '#') nadj[y][x]++;
				if (g[y2][x2] != '.') break;
			}

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x] == 'L' && !nadj[y][x])
			{ nchange++; g[y][x] = '#'; }
		else if (g[y][x] == '#' && nadj[y][x] >= adjrule)
			{ nchange++; g[y][x] = 'L'; }

	return nchange;
}

static void
run(int hitdepth, int adjrule)
{
	int x,y, nocc=0;

	dump();
	while (step(hitdepth, adjrule))
		dump();

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		nocc += g[y][x] == '#';

	printf("%d\n", nocc);
}

int
main()
{
	char *c;

	for (h=0; h<CAP; h++) {
		if (!fgets(g[h], CAP, stdin))
			break;
		if (!(c = strchr(g[h], '\n')))
			errx(1, "input to wide");
		if (h==1)
			w = c-g[h]; 
	}

	run(1, 4); reset();
	run(CAP, 5);
}
