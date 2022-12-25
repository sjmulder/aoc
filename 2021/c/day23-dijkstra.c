#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * !! WIP !!
 *
 * Todo:
 *  - dedup states
 *  - use binary heap
 */

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
	int i;

	for (i = MIN(room+2, hall+1); i < MAX(room+2, hall); i++)
		if (s.halls[i])
			return 0;

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
get_moves(struct state s, struct state *out)
{
	int n=0;
	int i,j,k;	/* room, hallway, slot */

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

		out[n++] = move_in(s,i,j,k);
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

		out[n++] = move_out(s,i,j,k);
	}

	return n;
}

static int
solve(struct state s)
{
#define QSZ (128*1024*1024)

	static struct state *queue;
	struct state *min, tmp;
	int nseen=0, nq=1, i;

	if (queue) {
		queue = malloc(sizeof(*queue) * QSZ);
		assert(queue);
	}

	queue[0] = s;

	while (nq) {
		if (++nseen % 1000 == 0) {
			printf("%d;%d;%d\n", nseen, nq,
			    queue[nq-1].score);
			fflush(stdout);
		}

		if (check_win(queue[nq-1]))
			return queue[nq-1].score;

		assert(nq + 2*4*7 < QSZ);
		nq += get_moves(queue[nq-1], &queue[nq-1]) -1;

		for (min=NULL, i=0; i<nq; i++)
			if (!min || queue[i].score < min->score)
				min = &queue[i];

		tmp = *min;
		*min = queue[nq-1];
		queue[nq-1] = tmp;

		//qsort(queue, nq, sizeof(*queue), cmp_score_desc);
	}

	return 0;
}

#if 0
static int
solve(struct state s, int reset)
{
	static int best=INT_MAX;
	struct state moves[2*4*7];
	int n,i;

	if (reset) best = INT_MAX;
	if (s.score >= best) return best;
	if (check_win(s)) return s.score;

	n = get_moves(s, moves);
	for (i=0; i<n; i++)
		best = solve(moves[i], 0);

	return best;
}
#endif

int
main()
{
	struct state s;
	int n, p1=0,p2=0;

	memset(&s, 0, sizeof(s));

	n = scanf(" %*s %*s ###%c#%c#%c#%c### #%c#%c#%c#%c",
	    &s.rooms[0][0], &s.rooms[1][0],
	    &s.rooms[2][0], &s.rooms[3][0],
	    &s.rooms[0][1], &s.rooms[1][1],
	    &s.rooms[2][1], &s.rooms[3][1]);
	assert(n == 8);

#if 1
	s.nslots = 2;
	p1 = solve(s);
#endif

#if 0
	for (i=0; i<4; i++)
		s.rooms[i][3] = s.rooms[i][1];
	s.rooms[0][1] = 'D'; s.rooms[0][2] = 'D';
	s.rooms[1][1] = 'C'; s.rooms[1][2] = 'B';
	s.rooms[2][1] = 'B'; s.rooms[2][2] = 'A';
	s.rooms[3][1] = 'A'; s.rooms[3][2] = 'C';

	s.nslots = 4;
	p2 = solve(s);
#endif

	printf("23: %d %d\n", p1, p2);
	return 0;
}
