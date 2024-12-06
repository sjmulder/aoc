#include "common.h"

#define GZ 134

struct world { char g[GZ][GZ]; int x,y, dir; };

static const char carets[] = "^>v<";
static const int dx[4] = {0,1,0,-1}, dy[4] = {-1,0,1,0};

static inline char *ahead(struct world *w) {
    return &w->g[w->y+dy[w->dir]][w->x+dx[w->dir]]; }
static inline int visited(char t) { return t && strchr(carets, t); }
static inline int traversible(char t) { return t=='.' || visited(t); }

/* new tile, previously visited tile, in a loop, out of bounds */
enum { ST_NEW, ST_SEEN, ST_LOOP, ST_END };

static int
step(struct world *w)
{
	char *cell;
	int is_new;

	assert(w->x >= 0); assert(w->x < GZ);
	assert(w->y >= 0); assert(w->y < GZ);

	cell = &w->g[w->y][w->x];

	if (!traversible(*cell))	/* out of bounds? */
		return ST_END;
	while (*ahead(w) == '#')	/* turn if needed */
		w->dir = (w->dir +1) %4;
	if (*cell == carets[w->dir])	/* walked here same dir? */
		return ST_LOOP;

	is_new = *cell == '.';
	*cell = carets[w->dir];
	w->x += dx[w->dir];
	w->y += dy[w->dir];

	return is_new ? ST_NEW : ST_SEEN;
}

int
main(int argc, char **argv)
{
	static struct world w0,w1,w2;
	int p1=0,p2=0, x,y, r,i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=1; y<GZ && fgets(w0.g[y]+1, GZ-1, stdin); y++)
		;

	for (y=0; y<GZ; y++)
	for (x=0; x<GZ; x++)
	for (i=0; i<4; i++)
		if (w0.g[y][x] == carets[i])
			{ w0.x=x; w0.y=y; w0.dir=i; goto found_start; }
found_start:
	w0.g[y][x] = '.';

	/* keep the clean copy of the grid (needed for part 2) */
	memcpy(&w1, &w0, sizeof(w1));

	/* part 1: trace the path and count unseen tiles */
	while ((r = step(&w1)) <= ST_SEEN)
		p1 += r == ST_NEW;

	/* part 2: try putting obstacles on each tile seen in p1 */
	for (y=0; y<GZ; y++)
	for (x=0; x<GZ; x++)
		if (visited(w1.g[y][x])) {
			memcpy(&w2, &w0, sizeof(w2));
			w2.g[y][x] = '#';
			while ((r = step(&w2)) <= ST_SEEN) ;
			p2 += r == ST_LOOP;
		}

	printf("06: %d %d\n", p1, p2);
	return 0;
}
