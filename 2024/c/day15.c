#include "common.h"

#define GZ 52

struct world {
	char g[GZ][GZ];
	int px,py;
};

static void
dump(struct world *w)
{
	int x,y;

	for (y=0; y<GZ && isprint(w->g[y][0]); y++) {
		for (x=0; x<GZ && isprint(w->g[y][x]); x++)
			putchar(x==w->px && y==w->py
			    ? '@' : w->g[y][x]);
		putchar('\n');
	}

	putchar('\n');
}

static int
push(struct world *w, int x0, int y0, int dx, int dy)
{
	int x,y;

	//printf("pushing from %d,%d to %d,%d\n", x0,y0, dx,dy);

	for (x=x0, y=y0; x>=0 && x<GZ && y>=0 && y<GZ; x+=dx, y+=dy)
		switch (w->g[y][x]) {
		case '.': w->g[y0][x0]='.'; w->g[y][x]='O'; return 1;
		case 'O': break;
		default: return 0;
		}
	
	return 0;
}

static void
move(struct world *w, int dx, int dy)
{
	int px1,py1;

	px1 = w->px+dx;
	py1 = w->py+dy;

	assert(w->px>=0); assert(w->px<GZ);
	assert(w->py>=0); assert(w->py<GZ);
	assert(px1>=0); assert(px1<GZ);
	assert(py1>=0); assert(py1<GZ);

	if (w->g[py1][px1] == '.' || push(w, px1,py1, dx,dy))
		{ w->px=px1; w->py=py1; }
}

static int
score(struct world *w)
{
	int acc=0, x,y;

	for (y=0; y<GZ && w->g[y][0]; y++)
	for (x=0; x<GZ && w->g[y][x]; x++)
		if (w->g[y][x] == 'O')
			acc += 100*y + x;

	return acc;
}

int
main(int argc, char **argv)
{
	static struct world w1;

	int p1=0,p2=0, y, c;
	char *p;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=0; fgets(w1.g[y], GZ, stdin); y++) {
		assert(y+1 < GZ);
		if (!w1.g[y][0] || w1.g[y][0]=='\n')
			break;
		if ((p = strchr(w1.g[y], '@')))
			{ w1.py=y; w1.px=p-w1.g[y]; *p='.'; }
	}

	//dump(&w1);

	while ((c = getchar()) != EOF) {
		//printf("move: %c\n", c);

		switch (c) {
		case '^': move(&w1, 0,-1); break;
		case 'v': move(&w1, 0, 1); break;
		case '<': move(&w1, -1,0); break;
		case '>': move(&w1,  1,0); break;
		}

		//dump(&w1);
	}

	p1 = score(&w1);

	printf("15: %d %d\n", p1, p2);
	return 0;
}
