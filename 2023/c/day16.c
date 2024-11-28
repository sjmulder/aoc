#include "common.h"

enum { NN, EE, SS, WW };

static char map[128][128], beams[128][128];
static int w, h;

static void
ray(int x, int y, int dir)
{
	int c;

	while (x>=0 && y>=0 && x<w && y<h) {
		if (beams[y][x] & (1 << dir))
			break;

		beams[y][x] |= 1 << dir;
		c = map[y][x];

		if (dir==NN && c=='/')  dir = EE; else
		if (dir==EE && c=='/')  dir = NN; else
		if (dir==SS && c=='/')  dir = WW; else
		if (dir==WW && c=='/')  dir = SS; else
		if (dir==NN && c=='\\') dir = WW; else
		if (dir==EE && c=='\\') dir = SS; else
		if (dir==SS && c=='\\') dir = EE; else
		if (dir==WW && c=='\\') dir = NN; else
		if (dir==NN && c=='-') { ray(x,y,WW); dir = EE; } else
		if (dir==SS && c=='-') { ray(x,y,WW); dir = EE; } else
		if (dir==EE && c=='|') { ray(x,y,NN); dir = SS; } else
		if (dir==WW && c=='|') { ray(x,y,NN); dir = SS; } 

		x += dir==EE ? 1 : dir==WW ? -1 : 0;
		y += dir==SS ? 1 : dir==NN ? -1 : 0;
	}
}

static int
score(int x, int y, int dir)
{
	int score=0;

	memset(beams, 0, sizeof(beams));
	ray(x, y, dir);

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		score += !!beams[y][x];
	
	return score;
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, n, x,y;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (; fgets(map[h], sizeof(*map), stdin); h++)
		assert(h+1 < (int)LEN(map));
	
	assert(h>0); w = (int)strlen(map[0])-1;
	assert(w>0);

	p1 = score(0, 0, EE);

	for (x=0; x<w; x++) {
		n = score(x,   0, SS); if (n>p2) p2 = n;
		n = score(x, h-1, NN); if (n>p2) p2 = n;
	}

	for (y=0; y<h; y++) {
		n = score(0,   y, EE); if (n>p2) p2 = n;
		n = score(w-1, y, WW); if (n>p2) p2 = n;
	}

	printf("16: %d %d\n", p1, p2);
	return 0;
}
