#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/*
 *  01 2 3 4 5 67   halls[i]
 *    0 1 2 3 4     rooms[i][0]
 *    0 1 2 3 4     rooms[i][1]
 */

struct state {
	char halls[7];
	char rooms[4][2];
	int score;
};

/* between rooms (top slot) and halls */
static int dists[4][7] = {
	{3,2,2,4,6,8,9},
	{5,4,2,2,4,6,8},
	{7,6,4,2,2,4,6},
	{9,8,6,4,2,2,4}
};

static int
check_win(struct state s)
{
	int i,j;

	for (i=0; i<4; i++)
	for (j=0; j<2; j++)
		if (s.rooms[i][j] != 'A'+i)
			return 0;
	
	return 1;
}

static int
check_route(struct state s, int room, int hall)
{
	int i;

	for (i = MIN(room+2, hall+1); i < MAX(room+2, hall); i++)
		if (s.halls[i])
			return 0;
	
	return 1;
}

static struct state
move_out(struct state s, int room, int hall)
{
	int slot, cost;
	char c;

	slot = !s.rooms[room][0];
	cost = dists[room][hall] + slot;

	for (c='A'; c < s.rooms[room][slot]; c++)
		cost *= 10;
	
	s.score += cost;
	s.halls[hall] = s.rooms[room][slot];
	s.rooms[room][slot] = 0;

	return s;
}

static struct state
move_in(struct state s, int room, int hall)
{
	int slot, cost;
	char c;

	slot = !s.rooms[room][1];
	cost = dists[room][hall] + slot;

	for (c='A'; c < s.halls[hall]; c++)
		cost *= 10;
	
	s.score += cost;
	s.rooms[room][slot] = s.halls[hall];
	s.halls[hall] = 0;

	return s;
}

static int
solve(struct state s)
{
	static char spaces[100];
	static int depth=0, best=INT_MAX;
	int i,j;

	if (!spaces[0])
		memset(spaces, ' ', sizeof(spaces));

	if (s.score > best)
		return best;
	if (check_win(s)) {
		//printf("%.*s END\n", depth, spaces);
		return s.score;
	}

	/* hall -> room */
	for (i=0; i<4; i++)
	for (j=0; j<7; j++) {
		 /* hall not occupied */
		if (!s.halls[j]) continue;
		 /* wrong room */
		if (s.halls[j] != 'A'+i) continue;
		 /* room full */
		if (s.rooms[i][0] && s.rooms[i][1]) continue;
		 /* someone waiting to get out */
		if (s.rooms[i][1] && s.rooms[i][1] != 'A'+i) continue;
		 /* someone in the way */
		if (!check_route(s,i,j)) continue;

		//printf("%.*s hall %d -> room %d\n", depth, spaces, j,i);

		depth++;
		best = solve(move_in(s,i,j));
		depth--;
	}

	/* room -> hall */
	for (i=0; i<4; i++)
	for (j=0; j<7; j++) {
		 /* room not occupied */
		if (!s.rooms[i][0] && !s.rooms[i][1]) continue;
		 /* hall occupied */
		if (s.halls[j]) continue;
		 /* already the right room */
		if (s.rooms[i][1] == 'A'+i && (!s.rooms[i][0] ||
		    s.rooms[i][0] == 'A'+i)) continue;
		 /* someone in the way */
		if (!check_route(s,i,j)) continue;

		//printf("%.*s room %d -> hall %d\n", depth, spaces, i,j);

		depth++;
		best = solve(move_out(s,i,j));
		depth--;
	}

	return best;
}

int
main()
{
	struct state s={};
	int n, p1;

	n = scanf(" %*s %*s ###%c#%c#%c#%c### #%c#%c#%c#%c",
	    &s.rooms[0][0], &s.rooms[1][0],
	    &s.rooms[2][0], &s.rooms[3][0],
	    &s.rooms[0][1], &s.rooms[1][1],
	    &s.rooms[2][1], &s.rooms[3][1]);
	assert(n == 8);

	p1 = solve(s);

	printf("23: %d\n", p1);
	return 0;
}
