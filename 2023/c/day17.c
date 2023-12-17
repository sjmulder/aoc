#include "common.h"

#define GW	148
#define GH	148
#define NSTEPS	11

enum { NN, EE, SS, WW };

static char map[GH][GW];
static int costs[GH][GW][4][NSTEPS]; /* y,x,dist,steps */
static int w,h;

static int
step_flood(int minstep, int maxstep)
{
	int x,y, dir, step;
	int x2,y2, dir2, step2;
	int nchange=0, cost;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
	for (dir=0; dir<4; dir++)
	for (dir2=0; dir2<4; dir2++)	/* new direction */
	for (step=0; step<=maxstep; step++) {
		if (!(x==0 && y==0 && dir==EE && step==0) &&
		    !costs[y][x][dir][step])
			continue;			/* no reach */
		if (abs(dir2-dir) == 2) continue;	/* no 180 */
		step2 = dir==dir2 ? step+1 : 1;
		if (step<minstep && dir!=dir2) continue;/* short */
		if (step2>maxstep) continue;		/* far */
		x2 = dir2==EE ? x+1 : dir2==WW ? x-1 : x;
		y2 = dir2==SS ? y+1 : dir2==NN ? y-1 : y;
		if (x2 < 0 || x2 >= w) continue;	/* OOB */
		if (y2 < 0 || y2 >= h) continue;	/* OOB */

		cost = costs[y][x][dir][step] + map[y2][x2]-'0';

		if (costs[y2][x2][dir2][step2] == 0 ||
		    costs[y2][x2][dir2][step2] > cost) {
			costs[y2][x2][dir2][step2]  = cost;
			nchange++;
		}
	}

	return nchange;
}

static int
solve(int stmin, int stmax)
{
	int best=INT_MAX, dir,st;

	memset(costs, 0, sizeof(costs));

	while (step_flood(stmin, stmax))
		;

	for (dir=0; dir<4; dir++)
	for (st=stmin; st<stmax; st++)
		if (costs[h-1][w-1][dir][st] &&
		    costs[h-1][w-1][dir][st] < best)
			best = costs[h-1][w-1][dir][st];

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
