#include "common.h"

#define SZY	48
#define SZX	144

static char map[SZY][SZX];
static int dist[SZY][SZX];
static int w,h, sx,sy, ex,ey;

static int max(int a, int b) { return a>b ? a : b; }

static void UNUSED
dump(void)
{
	int y,x;

	printf(" sz=%d,%d start=%d,%d end=%d,%d\n",
	    w,h, sx,sy, ex,ey);

	for (y=0; y<h; y++) {
		printf("%.*s", w, map[y]);

		for (x=0; x<w; x++) {
			if (dist[y][x] == INT_MAX)
				printf(" ??");
			else
				printf(" %2d", dist[y][x]);
		}

		putchar('\n');
	}
}

static int
step(int dx, int dy)
{
	int nchange=0, x,y;

	for (y = max(0, -dy); y < h && y+dy < h; y++)
	for (x = max(0, -dx); x < w && x+dx < w; x++)
		if (map[y][x] >= map[y+dy][x+dx]-1 &&
		    dist[y][x]-1 > dist[y+dy][x+dx]) {
			dist[y][x] = dist[y+dy][x+dx]+1;
			nchange++;
		}

	return nchange;
}

int
main(int argc, char **argv)
{
	char *lf;
	int x,y, nchange, p2=INT_MAX;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (fgets(map[h], SZX, stdin)) {
		if ((lf = strchr(map[h], '\n')))
			*lf = '\0';
		h++;
		assert(h < SZY);
	}

	assert(h);
	w = (int)strlen(map[0]);

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		dist[y][x] = INT_MAX;

		switch (map[y][x]) {
		case 'S': sx = x; sy = y; map[y][x] = 'a'; break;
		case 'E': ex = x; ey = y; map[y][x] = 'z'; break;
		}
	}

	dist[ey][ex] = 0;

	do {
		nchange  = step( 1,0);
		nchange += step(-1,0);
		nchange += step(0, 1);
		nchange += step(0,-1);
	} while (nchange);

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (map[y][x] == 'a' && dist[y][x] < p2)
			p2 = dist[y][x];

	printf("12: %d %d\n", dist[sy][sx], p2);
	return 0;
}

