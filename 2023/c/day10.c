#include "common.h"

#define GSZ	148

enum { NORTH, EAST, SOUTH, WEST };

/* indexed by entry->exit dir */
static const char shapes[4][5] = {"|F.7", "J-7.", ".L|J", "L.F-"};

static char input[GSZ][GSZ];
static char visited[GSZ][GSZ];
static int sx, sy;		/* start */

/*
 * Loads the grid into input[] at offset (1,1), leaving line endings and
 * nul terminators in place, and finds starting location (sx,sy).
 *
 * The (1,1) offset is so we can safely do x-1 etc everywhere.
 */
static void
parse(void)
{
	int y=1;
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
 */
static int
trace_loop(void)
{
	int dist=0, x,y, dir, dir0;

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

	input[sy][sx] = shapes[dir][dir0];
	assert(input[sy][sx] != '.');

	return dist;
}

static int
count_inside(void)
{
	int n=0, in, x,y;

	for (y=0; y<GSZ; y++)
	for (x=0, in=0; x<GSZ; x++)
		if (!visited[y][x])
			n += in;
		else if (strchr("J|L", input[y][x]))
			in = !in;

	return n;
}

int
main(int argc, char **argv)
{
	int p1,p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	parse();
	p1 = trace_loop() / 2;
	p2 = count_inside();

	printf("10: %d %d\n", p1, p2);
	return 0;
}
