#include "common.h"

#define GW 102
#define GH 52

struct world {
	char g[GH][GW];
	int px,py;
};

static void
dump(struct world *w)
{
	int x,y;

	for (y=0; y<GH && isprint(w->g[y][0]); y++) {
		for (x=0; x<GW && isprint(w->g[y][x]); x++)
			putchar(x==w->px && y==w->py
			    ? '@' : w->g[y][x]);
		putchar('\n');
	}

	putchar('\n');
}

static int
can_push(struct world *w, int x, int y, int dx, int dy)
{
	assert(x>=0); assert(x<GW);
	assert(y>=0); assert(y<GH);

	return
	    x+dx >= 0 && x+dx < GW &&
	    y+dy >= 0 && y+dy < GW &&
	    w->g[y][x] == 'O' &&
	    (w->g[y+dy][x+dx] == '.' || can_push(w, x+dx,y+dy, dx,dy));
}

/* check can_push() first! */
static void
push(struct world *w, int x, int y, int dx, int dy)
{
	assert(x>=0); assert(x<GW);
	assert(y>=0); assert(y<GH);
	assert(x+dx>=0); assert(x+dx<GW);
	assert(y+dy>=0); assert(y+dy<GH);

	assert(w->g[y][x] == 'O');

	if (w->g[y+dy][x+dx] != '.')
		push(w, x+dx,y+dy, dx,dy);

	w->g[y][x] = '.';
	w->g[y+dy][x+dx] = 'O';
}

static void
move(struct world *w, int dx, int dy)
{
	int px1,py1;

	px1 = w->px+dx;
	py1 = w->py+dy;

	assert(w->px>=0); assert(w->px<GW);
	assert(w->py>=0); assert(w->py<GH);
	assert(px1>=0); assert(px1<GW);
	assert(py1>=0); assert(py1<GH);

	if (w->g[py1][px1] == '.') {
		w->px=px1; w->py=py1;
	} else if (can_push(w, px1,py1, dx,dy)) {
		push(w, px1,py1, dx, dy);
		w->px=px1; w->py=py1;
	}
}

static int
score(struct world *w)
{
	int acc=0, x,y;

	for (y=0; y<GH && w->g[y][0]; y++)
	for (x=0; x<GW && w->g[y][x]; x++)
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

	for (y=0; fgets(w1.g[y], GW, stdin); y++) {
		assert(y+1 < GH);
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
