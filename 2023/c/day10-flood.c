#include "common.h"

#define GSZ	148

enum { NORTH, EAST, SOUTH, WEST };

static char input[GSZ][GSZ];
static char visited[GSZ][GSZ];
static char area[GSZ][GSZ];	/* of NW corner of x,y */
static int sx, sy;		/* start */

/*
 * Loads the grid into input[] at offset (1,1), leaving line endings and
 * nul terminators in place, and finds starting location (sx,sy).
 *
 * The (1,1) offset is so that we can safely do x-1 etc. everywhere.
 */
static void
parse(void)
{
	int y;
	char *start=NULL;

	/* otherwise strchr() will match the 0 bytes */
	memset(input, '.', sizeof(input));

	for (y=1; y<GSZ-1; y++) {
		if (!fgets(&input[y][1], LEN(*input)-2, stdin))
			break;
		if (!start && (start = strchr(&input[y][1], 'S'))) {
			sy = y;
			sx = start - &input[y][0];
		}
	}
}

/*
 * Follows the pipe from the start position, marking all tiles of the
 * loop in visited[], replacing (sx,sy) with the appropriate piece, and
 * returns the length of the loop.
 *
 * The area coloring is seeded by marking the NW corner of the starting
 * location as color 1 in area[] and an adjacent tile on the other side
 * of pipe as 2.
 */
static int
trace_loop(void)
{
	int dist, x,y, dir, dir0;

	x = sx;
	y = sy;
	visited[y][x] = 1;

	if (strchr("7|F", input[y-1][x]))
		{ y--; dir0 = dir = NORTH; }
	else if (strchr("J|L", input[y+1][x]))
		{ y++; dir0 = dir = SOUTH; }
	else if (strchr("L-F", input[y][x-1]))
		{ x--; dir0 = dir = WEST; }
	else if (strchr("J-7", input[y][x+1]))
		{ x++; dir0 = dir = EAST; }
	else
		assert(!"no route from start");

	for (dist=1; input[y][x] != 'S'; dist++) {
		visited[y][x] = 1;

		if ((dir == NORTH && input[y][x] == '|') ||
		    (dir == WEST  && input[y][x] == 'L') ||
		    (dir == EAST  && input[y][x] == 'J'))
			{ y--; dir = NORTH; }
		else if ((dir == SOUTH && input[y][x] == '|') ||
		         (dir == WEST  && input[y][x] == 'F') ||
		         (dir == EAST  && input[y][x] == '7'))
			{ y++; dir = SOUTH; }
		else if ((dir == NORTH && input[y][x] == '7') ||
		         (dir == SOUTH && input[y][x] == 'J') ||
		         (dir == WEST  && input[y][x] == '-'))
			{ x--; dir = WEST; }
		else if ((dir == NORTH && input[y][x] == 'F') ||
		         (dir == SOUTH && input[y][x] == 'L') ||
		         (dir == EAST  && input[y][x] == '-'))
			{ x++; dir = EAST; }
		else
			assert(!"no route");

		assert(y > 0); assert(y < GSZ-1);
		assert(x > 0); assert(x < GSZ-1);
	}

	input[sy][sx] =
	    dir == NORTH && dir0 == NORTH ? '|' :
	    dir == NORTH && dir0 == EAST  ? 'F' :
	    dir == NORTH && dir0 == WEST  ? '7' :
	    dir == EAST  && dir0 == NORTH ? 'J' :
	    dir == EAST  && dir0 == EAST  ? '-' :
	    dir == EAST  && dir0 == SOUTH ? '7' :
	    dir == SOUTH && dir0 == EAST  ? 'L' :
	    dir == SOUTH && dir0 == SOUTH ? '|' :
	    dir == SOUTH && dir0 == WEST  ? 'J' :
	    dir == WEST  && dir0 == NORTH ? 'L' :
	    dir == WEST  && dir0 == SOUTH ? 'F' :
	    dir == WEST  && dir0 == WEST  ? '-' : 0;

	assert(input[sy][sx]);

	area[sy][sx] = 1;

	switch (input[sy][sx]) {
	case 'L': area[sy][sx+1] = 2; break;
	case '7': area[sy+1][sx] = 2; break;
	default:  area[sy+1][sx+1] = 2; break;
	}

	return dist;
}

static int
blocks_horiz(int x, int y)
{
	return visited[y][x] && strchr("J|L", input[y][x]);
}

static int
blocks_vert(int x, int y)
{
	return visited[y][x] && strchr("J-7", input[y][x]);
}

/*
 * Iteratively propagate the coloring in area[] to adjacent tiles that
 * are not blocked by the loop.
 *
 * Since the loop could be going through a tile and split its area, we
 * keep track of the area of the NW corner of tiles. This allows the
 * coloring to propagate between '||' etc.
 */
static void
flood(void)
{
	int x,y, dirty=1;

	while (dirty)
	for (y=1, dirty=0; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		if (area[y][x])
			;
		else if (area[y-1][x] && !blocks_vert(x, y-1))
			{ dirty=1; area[y][x] = area[y-1][x]; }
		else if (area[y+1][x] && !blocks_vert(x, y))
			{ dirty=1; area[y][x] = area[y+1][x]; }
		else if (area[y][x-1] && !blocks_horiz(x-1, y))
			{ dirty=1; area[y][x] = area[y][x-1]; }
		else if (area[y][x+1] && !blocks_horiz(x, y))
			{ dirty=1; area[y][x] = area[y][x+1]; }
}

/*
 * Count tiles that are fully contained in the given area, that is, all
 * four corners are the same color.
 */
static int
count_area(int id)
{
	int n=0, x,y;

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		n += id == area[y][x] &&
		     id == area[y][x+1] &&
		     id == area[y+1][x] &&
		     id == area[y+1][x+1];

	return n;
}

int
main(int argc, char **argv)
{
	int p1,p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	parse(); p1 = trace_loop() / 2;
	flood(); p2 = count_area(area[1][2] == 1 ? 2 : 1);

	printf("10: %d %d\n", p1, p2);
	return 0;
}
