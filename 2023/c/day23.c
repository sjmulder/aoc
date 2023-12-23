#include "common.h"

#define GSZ 143

enum { NN, EE, SS, WW };

static const int dirs[][2] = {{0,-1},{1,0},{0,1},{-1,0}};
static char map[GSZ][GSZ];
static int w,h;

/* for meander() */
static uint8_t visited[GSZ][GSZ];
static int hist[GSZ*GSZ];	/* directions */
static int nhist;

static int
choose_dir(int x, int y, int dir0, int climb)	/* -1 if none */
{
	int x2,y2, dir;

	assert(x >= 0); assert(x < w);
	assert(y >= 0); assert(y < h);

	for (dir=dir0; dir<4; dir++)
		if ((climb || dir==NN || map[y][x]!='^') &&
		    (climb || dir==EE || map[y][x]!='>') &&
		    (climb || dir==SS || map[y][x]!='v') &&
		    (climb || dir==WW || map[y][x]!='<') &&
		    (x2 = x+dirs[dir][0]) >= 0 && x2<w &&
		    (y2 = y+dirs[dir][1]) >= 0 && y2<h &&
		    map[y2][x2] != '#' && !visited[y2][x2])
			return dir;

	return -1;
}

static int
meander(int climb)
{
	int worst=0, dir, x=1,y=0;

	memset(visited, 0, sizeof(visited));
	memset(hist, 0, sizeof(hist));
	nhist = 0;

	while (1) {
		assert(x >= 0); assert(x < w);
		assert(y >= 0); assert(y < h);

		if (x==w-2 && y==h-1)
			worst = MAX(worst, nhist);

		visited[y][x] = 1;
		dir = choose_dir(x,y, 0, climb);

		while (dir == -1 && nhist) {
			nhist--;
			visited[y][x] = 0;
			x -= dirs[hist[nhist]][0];
			y -= dirs[hist[nhist]][1];
			dir = choose_dir(x,y, hist[nhist]+1, climb);
		}

		if (dir == -1)
			break;

		x += dirs[dir][0];
		y += dirs[dir][1];
		assert(nhist < (int)LEN(hist));
		hist[nhist++] = dir;
	}

	return worst;
}

int
main(int argc, char **argv)
{
	int p1,p2, y;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (y=0; fgets(map[y], sizeof(map[y]), stdin); y++)
		assert(y+1 < GSZ);
	
	h = y;
	w = strlen(map[0])-1;

	assert(h > 2);
	assert(w > 2);
	assert(map[0][1] == '.');
	assert(map[0][w] == '\n');

	p1 = meander(0);
	p2 = meander(1);

	printf("23: %d %d\n", p1, p2);
	return 0;
}
