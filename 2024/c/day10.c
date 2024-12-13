#include "common.h"

#define GZ 43

/*
 * To avoid having to clear the 'seen' array after every search we mark
 * and check it with a per-search marker value ('id').
 */
static char g[GZ][GZ];
static short seen[GZ][GZ];

static int
score(int id, int x, int y, int p2)
{
	if (x<0 || x>=GZ ||
	    y<0 || y>=GZ || (!p2 && seen[y][x] == id))
		return 0;

	seen[y][x] = id;

	if (g[y][x] == '9')
		return 1;

	return
	    (g[y-1][x] == g[y][x]+1 ? score(id, x, y-1, p2) : 0) +
	    (g[y+1][x] == g[y][x]+1 ? score(id, x, y+1, p2) : 0) +
	    (g[y][x-1] == g[y][x]+1 ? score(id, x-1, y, p2) : 0) +
	    (g[y][x+1] == g[y][x]+1 ? score(id, x+1, y, p2) : 0);
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, id=1, x,y;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	for (y=0; y<GZ && fgets(g[y], GZ, stdin); y++)
		;

	for (y=0; y<GZ; y++)
	for (x=0; x<GZ; x++)
		if (g[y][x] == '0') {
			p1 += score(id++, x, y, 0);
			p2 += score(id++, x, y, 1);
		}

	printf("10: %d %d\n", p1, p2);
	return 0;
}
