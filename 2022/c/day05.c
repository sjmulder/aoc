#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define NSTACKS	10
#define MAXH	99

struct state {
	char stacks[NSTACKS][MAXH+1];
	int heights[NSTACKS];
};

static void
read_input(struct state *s)
{
	char buf[64], tmp;
	int len, pos, i,j,k;

	while (fgets(buf, sizeof(buf), stdin)) {
		len = strlen(buf);
		for (pos = 1; pos < len; pos += 4) {
			i = pos/4;
			assert(i < NSTACKS);
			assert(s->heights[i]+1 <= MAXH);
			if (isdigit(buf[pos])) break;
			if (isspace(buf[pos])) continue;
			s->stacks[i][s->heights[i]++] = buf[pos];
		}
		if (pos < len && isdigit(buf[pos]))
			break;
	}

	/* stacks are upside down, flip them */
	for (i=0; i < NSTACKS; i++)
	for (j=0; j < s->heights[i]/2; j++) {
		k = s->heights[i]-1-j;
		tmp = s->stacks[i][j];
		s->stacks[i][j] = s->stacks[i][k];
		s->stacks[i][k] = tmp;
	}
}

static void
move_p1(struct state *s, int from, int to, int count)
{
	int i;

	assert(count <= s->heights[from]);
	assert(s->heights[to] + count <= MAXH);

	for (i=0; i<count; i++) 
		s->stacks[to][s->heights[to]++] =
		    s->stacks[from][--s->heights[from]];
}

static void
move_p2(struct state *s, int from, int to, int count)
{
	assert(count <= s->heights[from]);
	assert(s->heights[to] + count <= MAXH);

	memmove(
	    &s->stacks[to][s->heights[to]],
	    &s->stacks[from][s->heights[from]-count],
	    count * sizeof(**s->stacks));

	s->heights[from] -= count;
	s->heights[to] += count;
}

static void
print_top(struct state *s)
{
	int i;

	for (i=0; i < NSTACKS; i++)
		if (s->heights[i])
			printf("%c", s->stacks[i][s->heights[i]-1]);
}

int
main(int argc, char **argv)
{
	static struct state p1, p2;
	int count, from, to;
	
	if (argc > 1)
		freopen(argv[1], "r", stdin);

	read_input(&p1);
	memcpy(&p2, &p1, sizeof(p2));

	while (scanf(" move %d from %d to %d", &count, &from, &to)==3) {
		assert(from >= 1 && from <= NSTACKS);
		assert(to >= 1 && to <= NSTACKS);
		move_p1(&p1, from-1, to-1, count);
		move_p2(&p2, from-1, to-1, count);
	}

	printf("05: "); print_top(&p1);
	printf(" ");    print_top(&p2); printf("\n");

	return 0;
}
