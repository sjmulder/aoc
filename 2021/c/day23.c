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
 *    0 1 2 3 4     rooms[i][2]
 *    0 1 2 3 4     rooms[i][3]
 */

struct state {
	char halls[7];
	char rooms[4][4];
	int nslots;
	int score;
};

/* between rooms (top slot) and halls */
static int dists[4][7] = {
	{3,2,2,4,6,8,9},
	{5,4,2,2,4,6,7},
	{7,6,4,2,2,4,5},
	{9,8,6,4,2,2,3}
};

static void
dump(struct state s)
{
	int i;

	printf("%c%c.%c.%c.%c.%c%c",
	    s.halls[0] ? s.halls[0] : '.',
	    s.halls[1] ? s.halls[1] : '.',
	    s.halls[2] ? s.halls[2] : '.',
	    s.halls[3] ? s.halls[3] : '.',
	    s.halls[4] ? s.halls[4] : '.',
	    s.halls[5] ? s.halls[5] : '.',
	    s.halls[6] ? s.halls[6] : '.');
	
	for (i=0; i<s.nslots; i++)
		printf("\n  %c %c %c %c",
		    s.rooms[0][i] ? s.rooms[0][i] : '.',
		    s.rooms[1][i] ? s.rooms[1][i] : '.',
		    s.rooms[2][i] ? s.rooms[2][i] : '.',
		    s.rooms[3][i] ? s.rooms[3][i] : '.');

	printf("  %d\n\n", s.score);
}

/* checks if OCCUPIED slots of room are correct */
static int
check_room(struct state s, int room)
{
	int i;

	for (i=s.nslots-1; i>=0 && s.rooms[room][i]; i--)
		if (s.rooms[room][i] != 'A'+room)
			return 0;
	
	return 1;
}

/* checks if ALL slots are occupied and correct */
static int
check_win(struct state s)
{
	int i,j;

	for (i=0; i<4; i++)
	for (j=0; j<s.nslots; j++)
		if (s.rooms[i][j] != 'A'+i)
			return 0;
	
	return 1;
}

static int
check_route(struct state s, int room, int hall)
{
	int roomx, hallx, i;

	roomx = room*2 +3;
	hallx = hall*2;

	//printf("room=%d, hall=%d,  roomx=%d, hallx=%d\n", room, hall,
	//    roomx, hallx);

	for (i = MIN(roomx,hallx)+1; i < MAX(roomx,hallx); i++) {
		//printf(" i=%d halls[i/2]=%c\n", i,
		//    s.halls[i/2] ? s.halls[i/2] : '.');

		if (!(i%2) && s.halls[i/2]) {
			//printf(" returning 0\n");
			return 0;
		}
	}

	//for (i = MIN(room+2, hall+1); i < MAX(room+2, hall); i++)
	//	if (s.halls[i])
	//		return 0;
	
	//printf(" returning 1\n");
	return 1;
}

static struct state
move_out(struct state s, int room, int hall, int slot)
{
	int cost;
	char c;

	cost = dists[room][hall] + slot;
	for (c='A'; c < s.rooms[room][slot]; c++)
		cost *= 10;
	
	s.score += cost;
	s.halls[hall] = s.rooms[room][slot];
	s.rooms[room][slot] = 0;

	return s;
}

static struct state
move_in(struct state s, int room, int hall, int slot)
{
	int cost;
	char c;

	cost = dists[room][hall] + slot;
	for (c='A'; c < s.halls[hall]; c++)
		cost *= 10;
	
	s.score += cost;
	s.rooms[room][slot] = s.halls[hall];
	s.halls[hall] = 0;

	return s;
}

static int
solve(struct state s, int reset)
{
	static char spaces[100];
	static int depth=0, best=INT_MAX;
	int i,j,k;	/* room, hallway, slot */

	if (reset)
		best = INT_MAX;

	if (!spaces[0])
		memset(spaces, ' ', sizeof(spaces));

	if (s.score >= best)
		return best;
	//dump(s);
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
		 /* someone in the way */
		if (!check_route(s,i,j)) continue;
		 /* room full */
		if (s.rooms[i][0]) continue;
		 /* someone waiting to get out */
		if (s.rooms[i][s.nslots-1] && !check_room(s,i))
			continue;

		for (k=s.nslots-1; s.rooms[i][k]; k--) ;

		//printf("%.*s hall %d -> room %d.%d\n", depth, spaces,
		//    j,i,k);

		depth++;
		best = solve(move_in(s,i,j,k), 0);
		depth--;
	}

	/* room -> hall */
	for (i=0; i<4; i++)
	for (j=0; j<7; j++) {
		 /* room not occupied */
		if (!s.rooms[i][s.nslots-1]) continue;
		 /* hall occupied */
		if (s.halls[j]) continue;
		 /* all already the right room */
		if (check_room(s,i)) continue;
		 /* someone in the way */
		if (!check_route(s,i,j)) continue;

		for (k=0; !s.rooms[i][k]; k++) ;

		//printf("%.*s room %d.%d -> hall %d\n", depth, spaces,
		//    i,k,j);

		depth++;
		best = solve(move_out(s,i,j,k), 0);
		depth--;
	}

	return best;
}

int
main()
{
	struct state s={};
	int n,i, p1=0,p2=0;

#if 1
	n = scanf(" %*s %*s ###%c#%c#%c#%c### #%c#%c#%c#%c",
	    &s.rooms[0][0], &s.rooms[1][0],
	    &s.rooms[2][0], &s.rooms[3][0],
	    &s.rooms[0][1], &s.rooms[1][1],
	    &s.rooms[2][1], &s.rooms[3][1]);
	assert(n == 8);
#endif

#if 0
	s.halls[6] = 'C';

	s.rooms[0][0] = 'A';
	s.rooms[0][1] = 'A';
	s.rooms[0][2] = 'A';
	s.rooms[0][3] = 'A';
	s.rooms[1][0] = 'B';
	s.rooms[1][1] = 'B';
	s.rooms[1][2] = 'B';
	s.rooms[1][3] = 'B';
	s.rooms[2][1] = 'C';
	s.rooms[2][2] = 'C';
	s.rooms[2][3] = 'C';
	s.rooms[3][0] = 'D';
	s.rooms[3][1] = 'D';
	s.rooms[3][2] = 'D';
	s.rooms[3][3] = 'D';
#endif

#if 1
	s.nslots = 2;
	//dump(s);
	p1 = solve(s, 1);
#endif

#if 1
	for (i=0; i<4; i++)
		s.rooms[i][3] = s.rooms[i][1];
	s.rooms[0][1] = 'D'; s.rooms[0][2] = 'D';
	s.rooms[1][1] = 'C'; s.rooms[1][2] = 'B';
	s.rooms[2][1] = 'B'; s.rooms[2][2] = 'A';
	s.rooms[3][1] = 'A'; s.rooms[3][2] = 'C';
#endif

	s.nslots = 4;
	//dump(s);
	p2 = solve(s, 1);

	printf("23: %d %d\n", p1, p2);
	return 0;
}
