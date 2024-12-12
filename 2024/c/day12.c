#include "common.h"

#define GZ 144
static char g[GZ][GZ];
static char seen[GZ][GZ];

static void
count(char c, int x, int y, int *area, int *perim, int *sides)
{
	if (g[y][x] != c) { (*perim)++; return; }
	if (seen[y][x]) return;

	*area += 1;
	seen[y][x] = 1;

	/* count start of top/left edges, end of bottom/right edges */
	*sides += g[y-1][x]!=c && ((g[y-1][x-1]==c) || (g[y][x-1]!=c));
	*sides += g[y+1][x]!=c && ((g[y+1][x+1]==c) || (g[y][x+1]!=c));
	*sides += g[y][x-1]!=c && ((g[y-1][x-1]==c) || (g[y-1][x]!=c));
	*sides += g[y][x+1]!=c && ((g[y+1][x+1]==c) || (g[y+1][x]!=c));

	count(c, x, y-1, area, perim, sides);
	count(c, x, y+1, area, perim, sides);
	count(c, x-1, y, area, perim, sides);
	count(c, x+1, y, area, perim, sides);
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, x,y, area, perim, sides;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=1; fgets(g[y]+1, GZ-2, stdin); y++)
		assert(y+1 < GZ);

	for (y=1; y<GZ-1; y++)
	for (x=1; x<GZ-1; x++)
		if (isalpha(g[y][x]) && !seen[y][x]) {
			area  = perim = sides = 0;
			count(g[y][x], x, y, &area, &perim, &sides);
			p1 += area * perim;
			p2 += area * sides;
		}

	printf("12: %d %d\n", p1, p2);
}
