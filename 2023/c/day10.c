#include "common.h"

#define GSZ	148

enum { NORTH, EAST, SOUTH, WEST };

/* indexed by entry->exit dir */
static const char shapes[4][5] = {"|F.7", "J-7.", ".L|J", "L.F-"};

/* indexed by char, cur dir */
static const int moves[256][4] = {
    ['|'] = {NORTH,     0, SOUTH,     0},
    ['-'] = {    0,  EAST,     0,  WEST},
    ['L'] = {    0,     0,  EAST, NORTH},
    ['J'] = {    0, NORTH,  WEST,     0},
    ['7'] = { WEST, SOUTH,     0,     0},
    ['F'] = { EAST,     0,     0, SOUTH}
};

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
		if (!start && (start = strchr(&input[y][1], 'S')))
			{ sy = y; sx = start - &input[y][0]; }
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
	int dist=0, x=sx,y=sy, dir,dir0;

	dir0 = dir =
	    strchr("7|F", input[y-1][x]) ? NORTH :
	    strchr("J|L", input[y+1][x]) ? SOUTH :
	    strchr("L-F", input[y][x-1]) ? WEST :
	    strchr("J-7", input[y][x+1]) ? EAST : -1;
	assert(dir != -1);

	for (; !visited[y][x]; dist++) {
		visited[y][x] = 1;
		x += dir==EAST  ? 1 : dir==WEST  ? -1 : 0;
		y += dir==SOUTH ? 1 : dir==NORTH ? -1 : 0;
		assert(y > 0); assert(y < GSZ-1);
		assert(x > 0); assert(x < GSZ-1);
		dir = moves[(uint8_t)input[y][x]][dir];
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
