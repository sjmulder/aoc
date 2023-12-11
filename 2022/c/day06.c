#include "common.h"

/*
 * We keep a ring buffer of the last N characters (4 for part 1,
 * 14 for part 2). To avoid O(n!) time complexity for the
 * duplicate check, we track the counts-per-char and use that to
 * track the number of duplicates.
 */
struct state {
	char *ring;	/* pointer to a ring buffer */
	char counts[64];/* char%64->count map (ok for letters) */
	int sz, pos;
	int ndupes;	/* number of duplicate chars */
	int ans;	/* the solution */
};

static void
step(struct state *s, char c)
{
	int i;
	char old_c;

	i = s->pos % s->sz;

	/* remove old character, decrement ndupes if needed */
	if ((old_c = s->ring[i]))
		s->ndupes -= --s->counts[old_c % 64] == 1;

	/* add the new character */
	s->ring[i] = c;
	s->ndupes += ++s->counts[c%64] == 2;

	/* if no dupes, we have our answer */
	if (s->pos >= s->sz && !s->ndupes)
		s->ans = s->pos+1;

	s->pos++;
}

int
main(int argc, char **argv)
{
	static char buf1[4], buf2[14];
	static struct state p1, p2;
	int c;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	p1.ring = buf1; p1.sz = 4;
	p2.ring = buf2; p2.sz = 14;

	while ((!p1.ans || !p2.ans) && (c = getchar()) != EOF) {
		if (!p1.ans) step(&p1, (char)c);
		if (!p2.ans) step(&p2, (char)c);
	}

	printf("06: %d %d\n", p1.ans, p2.ans);
	return 0;
}
