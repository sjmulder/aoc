#include "common.h"

#define GW 148
#define GH 148
#define ND 4
#define NS 10

enum { NN, EE, SS, WW };

struct node { int x,y,d,s; };

static char map[GH][GW];
static int costs[GH][GW][ND][NS];	/* y,x,dist,steps */
static int ages[GH][GW][ND][NS];
static int w,h, cur_age;

static int
is_valid(struct node a, struct node b, int smin, int smax)
{
	return
	    b.x >= 0 && b.x < w &&
	    b.y >= 0 && b.y < h &&
	    abs(b.d-a.d) != 2 &&
	    (a.s+1 >= smin || a.d == b.d) &&
	     b.s+1 <= smax;
}

static struct node
node_from(struct node a, int d)
{
	struct node b;

	b.x = d==EE ? a.x+1 : d==WW ? a.x-1 : a.x;
	b.y = d==SS ? a.y+1 : d==NN ? a.y-1 : a.y;
	b.d = d;
	b.s = d==a.d ? a.s+1 : 0;

	return b;
}

static int
step_flood(int smin, int smax)
{
	struct node a, b;
	int nchange=0, a_cost,b_cost,b_cur, d;

	for (a.y=0; a.y<h; a.y++)
	for (a.x=0; a.x<w; a.x++)
	for (a.d=0; a.d<ND; a.d++)
	for (a.s=0; a.s<smax; a.s++) {
		if (ages[a.y][a.x][a.d][a.s] < cur_age-1)
			continue;
		if ((a_cost = costs[a.y][a.x][a.d][a.s]) == INT_MAX)
			continue;
		for (d=0; d<ND; d++) {
			b = node_from(a, d);

			if (!is_valid(a, b, smin, smax))
				continue;

			b_cost = a_cost + map[b.y][b.x]-'0';
			b_cur = costs[b.y][b.x][b.d][b.s];

			if (b_cost < b_cur) {
				costs[b.y][b.x][b.d][b.s] = b_cost;
				ages[b.y][b.x][b.d][b.s] = cur_age;
				nchange++;
			}
		}
	}

	cur_age++;

	return nchange;
}

static int
solve(int smin, int smax)
{
	int best=INT_MAX, x,y,d,s;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
	for (d=0; d<ND; d++)
	for (s=0; s<smax; s++) {
		costs[y][x][d][s] = INT_MAX;
		ages[y][x][d][s] = 0;
	}

	cur_age = 0;
	costs[0][1][EE][0] = 0;

	while (step_flood(smin, smax))
		;

	for (d=0; d<ND; d++)
	for (s=smin; s<smax; s++)
		if (costs[h-1][w-1][d][s] &&
		    costs[h-1][w-1][d][s] < best)
			best = costs[h-1][w-1][d][s];

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
	
	assert(h>0); w = (int)strlen(map[0])-1;
	assert(w>0);

	p1 = solve(0, 3);
	p2 = solve(4, 10);

	printf("17: %d %d\n", p1, p2);
	return 0;
}
