#include "common.h"

#define GW 148
#define GH 148
#define ND 4
#define NS 11

enum { NN, EE, SS, WW };

struct node { int x,y,d,s; };

static char map[GH][GW];
static int costs[GH][GW][ND][NS]; /* y,x,dist,ss */
static int w,h;

static int
is_valid(struct node a, struct node b, int smin, int smax)
{
	return
	    b.x >= 0 && b.x < w &&
	    b.y >= 0 && b.y < h &&
	    abs(b.d-a.d) != 2 &&
	    (a.s >= smin || a.d == b.d) &&
	     b.s <= smax;
}

static struct node
node_from(struct node a, int d)
{
	struct node b;

	b.x = d==EE ? a.x+1 : d==WW ? a.x-1 : a.x;
	b.y = d==SS ? a.y+1 : d==NN ? a.y-1 : a.y;
	b.d = d;
	b.s = d==a.d ? a.s+1 : 1;

	return b;
}

static int
get_cost(struct node n)
{
	int cost;

	cost = costs[n.y][n.x][n.d][n.s];

	return cost || (n.x==0 && n.y==0 && n.d==EE && n.s==0)
	    ? cost : INT_MAX;
}

static void
set_cost(struct node n, int cost)
{
	costs[n.y][n.x][n.d][n.s] = cost;
}

static int
step_flood(int smin, int smax)
{
	struct node a, b;
	int nchange=0, cost, d;

	for (a.y=0; a.y<h; a.y++)
	for (a.x=0; a.x<w; a.x++)
	for (a.d=0; a.d<ND; a.d++)
	for (a.s=0; a.s <= smax; a.s++)
	for (d=0; d<ND; d++)
		if ((cost = get_cost(a)) != INT_MAX &&
		    is_valid(a, (b = node_from(a, d)), smin, smax) &&
		    (cost += map[b.y][b.x]-'0') < get_cost(b)) {
			set_cost(b, cost);
			nchange++;
		}

	return nchange;
}

static int
solve(int smin, int smax)
{
	int best=INT_MAX, d,st;

	memset(costs, 0, sizeof(costs));

	while (step_flood(smin, smax))
		;

	for (d=0; d<ND; d++)
	for (st=smin; st<smax; st++)
		if (costs[h-1][w-1][d][st] &&
		    costs[h-1][w-1][d][st] < best)
			best = costs[h-1][w-1][d][st];

	return best;
}

int
main(int argc, char **argv)
{
	int p1, p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (; fgets(map[h], GW, stdin); h++)
		assert(h+1 < GH);
	
	assert(h>0); w = strlen(map[0])-1;
	assert(w>0);

	p1 = solve(0, 3);
	p2 = solve(4, 10);

	printf("17: %d %d\n", p1, p2);
	return 0;
}
