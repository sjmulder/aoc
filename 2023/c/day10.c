#include "common.h"

/*
 * Updated, much shorter solution using edge counting for part 2. For
 * original solution with flood fill area coloring, see day10-flood.c
 */

#define GSZ 148

enum {NN, EE, SS, WW};

/* indexed by entry->exit dir */
static const char shapemap[4][5] = {"|F.7", "J-7.", ".L|J", "L.F-"};

/* moves indexed by shape idx, cur dir */
static const char shapes[] = "|-LJ7F";
static const int moves[6][4] = {
    {NN,0,SS,0}, {0,EE,0,WW}, {0,0,EE,NN},
    {0,NN,WW,0}, {WW,SS,0,0}, {EE,0,0,SS}
};

int
main(int argc, char **argv)
{
	static char map[GSZ][GSZ];
	static char visited[GSZ][GSZ];
	int dist,nin, sx=0,sy=0, x,y, dir,dir0, in;
	char *start=NULL, *s;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	/* otherwise strchr() will match the 0 bytes */
	memset(map, '.', sizeof(map));

	for (y=1; y<GSZ-1; y++) {
		if (!fgets(&map[y][1], LEN(*map)-2, stdin))
			break;
		if (!start && (start = strchr(&map[y][1], 'S')))
			{ sy = y; sx = start - &map[y][0]; }
	}

	assert(sx); x=sx;
	assert(sy); y=sy;

	dir0 = dir =
	    strchr("7|F", map[y-1][x]) ? NN :
	    strchr("J|L", map[y+1][x]) ? SS :
	    strchr("L-F", map[y][x-1]) ? WW :
	    strchr("J-7", map[y][x+1]) ? EE : -1;
	assert(dir != -1);

	for (dist=0; !visited[y][x]; dist++) {
		visited[y][x] = 1;
		x += dir==EE ? 1 : dir==WW ? -1 : 0;
		y += dir==SS ? 1 : dir==NN ? -1 : 0;
		assert(y > 0); assert(y < GSZ-1);
		assert(x > 0); assert(x < GSZ-1);
		if ((s = strchr(shapes, map[y][x])))
			dir = moves[s-shapes][dir];
	}

	map[sy][sx] = shapemap[dir][dir0];
	assert(map[sy][sx] != '.');

	for (y=0, nin=0; y<GSZ; y++)
	for (x=0, in=0; x<GSZ; x++)
		if (!visited[y][x])
			nin += in;
		else if (strchr("J|L", map[y][x]))
			in = !in;

	printf("10: %d %d\n", dist/2, nin);
	return 0;
}
