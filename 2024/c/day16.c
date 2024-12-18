#include "common.h"

#define GZ 145

enum {NN, EE, SS, WW};

static const int dx[]={0,1,0,-1}, dy[]={-1,0,1,0};

static char g[GZ][GZ];		/* with 1 tile border */
static int cost[GZ][GZ][4];	/* per dir, starts at 1, 0=no info */

static int traversible(char c) { return c=='.' || c=='S' || c=='E'; }

static int
minat(int x, int y)
{
	int acc=0, d;

	for (d=0; d<4; d++)
		if (cost[y][x][d] && (!acc || cost[y][x][d] < acc))
			acc = cost[y][x][d];

	return acc;
}


static int
count_exits(int x, int y)
{
	int acc=0, i;

	assert(x>0); assert(x<GZ-1);
	assert(y>0); assert(y<GZ-1);

	for (i=0; i<4; i++)
		acc += traversible(g[y+dy[i]][x+dx[i]]);
	
	return acc;
}

/* remove all dead ends */
static void
prune_dead(void)
{
	int dirty=1, x,y;

	while (dirty) {
		dirty = 0;

		for (y=1; y<GZ-1; y++)
		for (x=1; x<GZ-1; x++)
			if (g[y][x]=='.' && count_exits(x,y) < 2)
				{ dirty = 1; g[y][x] = '#'; }
	}
}

/* remove all dead ends from cost[], leaves only optimal paths */
static void
prune_subopt(void)
{
	int dirty=1, x,y,d;

	while (dirty) {
		dirty = 0;

		for (y=1; y<GZ-1; y++)
		for (x=1; x<GZ-1; x++)
		for (d=0; d<4; d++) {
			if (!cost[y][x][d])
				continue;

			if (g[y][x]=='E') {
				if (cost[y][x][d] != minat(x,y))
					{ dirty = 1; cost[y][x][d] = 0; }
				continue;
			}

			if (cost[y][x][d]+1 > cost[y+dy[d]][x+dx[d]][d] &&
			    cost[y][x][d]+1000 > cost[y][x][(d+1)%4] &&
			    cost[y][x][d]+1000 > cost[y][x][(d+3)%4])
				{ dirty = 1; cost[y][x][d] = 0; }
		}
	}
}

static void
propagate_costs(void)
{
	int dirty=1, cost1, x,y,d;

	while (dirty) {
		dirty = 0;

		for (y=1; y<GZ-1; y++)
		for (x=1; x<GZ-1; x++)
		for (d=0; d<4; d++) {
			if (!traversible(g[y][x]))
				continue;

			/* from back */
			if ((cost1 = cost[y-dy[d]][x-dx[d]][d]) &&
			    (cost1+1 < cost[y][x][d] || !cost[y][x][d]))
				{ dirty = 1; cost[y][x][d] = cost1+1; }

			/* from right */
			if ((cost1 = cost[y][x][(d+1)%4]) &&
			    (cost1+1000 < cost[y][x][d] || !cost[y][x][d]))
				{ dirty = 1; cost[y][x][d] = cost1+1000; }

			/* from left */
			if ((cost1 = cost[y][x][(d+3)%4]) &&
			    (cost1+1000 < cost[y][x][d] || !cost[y][x][d]))
				{ dirty = 1; cost[y][x][d] = cost1+1000; }
		}
	}
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, sx=0,sy=0, ex=0,ey=0, x,y;
	char *p;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=1; fgets(g[y]+1, GZ-1, stdin); y++) {
		if ((p = strchr(g[y]+1, 'S'))) {sy=y; sx=(int)(p-g[y]);}
		if ((p = strchr(g[y]+1, 'E'))) {ey=y; ex=(int)(p-g[y]);}
		assert(y+1 < GZ-1);
	}

	cost[sy][sx][EE] = 1;

	prune_dead();
	propagate_costs();
	prune_subopt();

	p1 = minat(ex, ey) -1;	/* costs[] values start at 1! */

	for (y=1; y<GZ-1; y++)
	for (x=1; x<GZ-1; x++)
		p2 += minat(x,y) > 0;

	printf("16: %d %d\n", p1, p2);
	return 0;
}
