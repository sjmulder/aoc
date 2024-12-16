#include "common.h"

#define GW 104
#define GH 52

struct world { char g[GH][GW]; int px,py; };

static int
can_clear(struct world *w, int x, int y, int dx, int dy)
{
	assert(x>=0); assert(x<GW);
	assert(y>=0); assert(y<GH);
	assert((dx && !dy) || (dy && !dx));

	return
	    (x+dx >= 0 || x+dx < GW) &&
	    (y+dy >= 0 || y+dy < GW) &&
	    (w->g[y][x] == '.' || (
	     w->g[y][x] != '#' && can_clear(w, x+dx,y+dy, dx,dy) &&
	     (!dy || w->g[y][x]!='[' || can_clear(w, x+1,y+dy, 0,dy)) &&
	     (!dy || w->g[y][x]!=']' || can_clear(w, x-1,y,    0,dy)) &&
	     (!dy || w->g[y][x]!=']' || can_clear(w, x-1,y+dy, 0,dy))));
}

/* check can_clear() first! */
static void
clear(struct world *w, int x, int y, int dx, int dy)
{
	assert(x>=0); assert(x<GW); assert(x+dx>=0); assert(x+dx<GW);
	assert(y>=0); assert(y<GH); assert(y+dy>=0); assert(y+dy<GH);

	if (w->g[y][x] == '.')
		return;
	if (dy && w->g[y][x] == ']')
		{ clear(w, x-1,y, dx,dy); return; }

	if (dy && w->g[y][x] == '[') {
		clear(w, x+1,y+dy, dx,dy);
		w->g[y+dy][x+dx+1] = ']';
		w->g[y][x+1] = '.';
	}

	clear(w, x+dx,y+dy, dx,dy);
	w->g[y+dy][x+dx] = w->g[y][x];
	w->g[y][x] = '.';
}

static void
move(struct world *w, int dx, int dy)
{
	if (can_clear(w, w->px+dx, w->py+dy, dx,dy)) {
		clear(w, w->px+dx, w->py+dy, dx,dy);
		w->px += dx;
		w->py += dy;
	}
}

static int
score(struct world *w)
{
	int acc=0, x,y;

	for (y=0; y<GH && w->g[y][0]; y++)
	for (x=0; x<GW && w->g[y][x]; x++)
		if (w->g[y][x] == 'O' || w->g[y][x] == '[')
			acc += 100*y + x;

	return acc;
}

int
main(int argc, char **argv)
{
	static struct world w1,w2;
	int x,y, c;
	char *p;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=0; fgets(w1.g[y], GW, stdin); y++) {
		if (!w1.g[y][0] || w1.g[y][0]=='\n')
			break;

		assert(y+1 < GH);
		assert(strlen(w1.g[y])*2+1 < GW);

		for (x=0; w1.g[y][x]; x++)
			if (w1.g[y][x] == 'O') {
				w2.g[y][x*2]   = '[';
				w2.g[y][x*2+1] = ']';
			} else {
				w2.g[y][x*2]   = w1.g[y][x];
				w2.g[y][x*2+1] = w1.g[y][x];
			}

		if ((p = strchr(w1.g[y], '@'))) {
			w1.py = y; w1.px = p-w1.g[y];
			w2.py = y; w2.px = w1.px*2;

			w1.g[w1.py][w1.px]   = '.';
			w2.g[w2.py][w2.px]   = '.';
			w2.g[w2.py][w2.px+1] = '.';
		}
	}

	while ((c = getchar()) != EOF)
		switch (c) {
		case '^': move(&w1, 0,-1); move(&w2, 0,-1); break;
		case 'v': move(&w1, 0, 1); move(&w2, 0, 1); break;
		case '<': move(&w1,-1, 0); move(&w2,-1, 0); break;
		case '>': move(&w1, 1, 0); move(&w2, 1, 0); break;
		}

	printf("15: %d %d\n", score(&w1), score(&w2));
	return 0;
}
