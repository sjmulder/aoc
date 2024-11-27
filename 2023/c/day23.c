#include "common.h"

#define GSZ 143

/*
 * Implemented with a junction graph. Some assumptions:
 *  - No dead ens
 *  - No rooms (only corridors and junctions)
 *  - Slopes only at junction exits
 */

enum { NN, EE, SS, WW };

struct node;
struct link { struct node *node; int dist; };
struct node { int x,y, visited; struct link links[4]; };

static const int dirs[][2] = {{0,-1},{1,0},{0,1},{-1,0}};
static char map[GSZ][GSZ];
static int w,h;

static struct node nodes[40];
static int nnodes;

static struct node *
node_at(int x, int y)
{
	int i;

	for (i=0; i<nnodes; i++)
		if (nodes[i].x == x && nodes[i].y == y)
			return &nodes[i];

	assert(nnodes < (int)LEN(nodes));
	nodes[i].x = x;
	nodes[i].y = y;
	nnodes++;

	return &nodes[i];
}

static int
can_move(int x, int y, int dir, int climb)
{
	int x2,y2;

	assert(x >= 0); assert(x < w);
	assert(y >= 0); assert(y < h);

	return
	    (x2 = x+dirs[dir][0]) >= 0 && x2<w &&
	    (y2 = y+dirs[dir][1]) >= 0 && y2<h &&
	    (climb || dir!=NN || map[y2][x2]!='v') &&
	    (climb || dir!=EE || map[y2][x2]!='<') &&
	    (climb || dir!=SS || map[y2][x2]!='^') &&
	    (climb || dir!=WW || map[y2][x2]!='>') &&
	    map[y2][x2] != '#';
}

static void
survey(void)
{
	struct node *src, *dst;
	int dir0,dir,rdir=0, ndirs, dist, x,y, i,j;

	nnodes = 0;
	memset(nodes, 0, sizeof(nodes));
	node_at(1,0);	/* create initial node */

	for (i=0; i<nnodes; i++)
	for (dir0=0; dir0<4; dir0++) {
		src = &nodes[i];
		x = src->x;
		y = src->y;
		dir = dir0;
		dist = 0;

		if (src->links[dir].node || !can_move(x,y, dir, 1))
			continue;

		do {
			rdir = (dir+2) % 4;
			x += dirs[dir][0];
			y += dirs[dir][1];
			dist++;

			if (x==w-2 && y==h-1)
				break;

			assert(x >= 0); assert(x < w);
			assert(y >= 0); assert(y < h);
			assert(map[y][x] != '#');

			for (dir=0; dir<4; dir++)
				if (dir!=rdir && can_move(x,y, dir, 1))
					break;

			assert(dir != 4); /* no dead ends */

			/* junction if ndirs > 2 */
			for (j=0, ndirs=0; j<4; j++)
				ndirs += can_move(x,y, j, 1);
		} while (ndirs <= 2);

		dst = node_at(x,y);
		src->links[dir0].node = dst;
		src->links[dir0].dist = dist;
		dst->links[rdir].node = src;
		dst->links[rdir].dist = dist;
	}
}

static int
meander(struct node *node, int dir, int climb)
{
	struct node *dst;
	int worst=-1, dist, i;

	if (node->x == w-2 && node->y == h-1)
		return 0;
	if (node->visited || !can_move(node->x, node->y, dir, climb))
		return -1;
	if (!(dst = node->links[dir].node))
		return -1;

	node->visited = 1;

	for (i=0; i<4; i++) {
		dist = meander(dst, i, climb);
		worst = MAX(dist, worst);
	}

	node->visited = 0;
	return worst == -1 ? -1 : node->links[dir].dist + worst;
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
	w = (int)strlen(map[0])-1;

	assert(h > 2);
	assert(w > 2);
	assert(map[0][1] == '.');
	assert(map[0][w] == '\n');

	survey();

	p1 = meander(node_at(1,0), SS, 0);
	p2 = meander(node_at(1,0), SS, 1);

	printf("23: %d %d\n", p1, p2);
	return 0;
}
