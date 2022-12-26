#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct mon {
	char name[5], op, dep_x;	/* dep_x: depends on 'x'? */
	struct mon *deps[2], *x;	/* x: cache for depends() */
	int64_t val;
};

static struct mon mons[2560], *root, *humn;
size_t nm;

static struct mon *
by_name(char name[5])
{
	size_t i;

	for (i=0; i<nm; i++)
		if (!strcmp(name, mons[i].name))
			return &mons[i];

	assert(nm < LEN(mons));
	memcpy(mons[nm].name, name, sizeof(mons[nm].name));
	return &mons[nm++];
}

static int
depends(struct mon *m, struct mon *x)
{
	if (m == x) return 1;
	if (m->x == x) return m->dep_x;
	if (m->op == 0) return 0;

	m->x = x;
	m->dep_x = depends(m->deps[0], x) || depends(m->deps[1], x);
	return m->dep_x;
}

static int64_t
eval(struct mon *m)
{
	switch (m->op) {
	case 0: return m->val;
	case '+': return eval(m->deps[0]) + eval(m->deps[1]);
	case '-': return eval(m->deps[0]) - eval(m->deps[1]);
	case '*': return eval(m->deps[0]) * eval(m->deps[1]);
	case '/': return eval(m->deps[0]) / eval(m->deps[1]);
	default: assert(!"bad op"); return 0;
	}
}

/* what must 'x' be for 'm' to equal 'val'? */
static int64_t
solve(struct mon *m, struct mon *x, int64_t val)
{
	struct mon *d0 = m->deps[0], *d1 = m->deps[1];

	if (m == x)
		return val;

	if (depends(d0, x))	/* x in left operand */
		switch (m->op) {
		case '+': return solve(d0, x, val - eval(d1));
		case '-': return solve(d0, x, val + eval(d1));
		case '*': return solve(d0, x, val / eval(d1));
		case '/': return solve(d0, x, val * eval(d1));
		default: assert(!"bad op"); return 0;
		}
	else			/* x in right operand */
		switch (m->op) {
		case '+': return solve(d1, x, val - eval(d0));
		case '*': return solve(d1, x, val / eval(d0));
		case '-': return solve(d1, x, eval(d0) - val);
		case '/': return solve(d1, x, eval(d0) / val);
		default: assert(!"bad op"); return 0;
		}
}

static void
read_input(void)
{
	static char b[24];
	struct mon *m;
	char *s, *nm, d1[5], d2[5], c;

	while ((s = fgets(b, sizeof(b), stdin))) {
		nm = strsep(&s, ":");
		assert(strlen(nm) == 4);

		m = by_name(nm);
		if (!strcmp(nm, "root")) root = m; else
		if (!strcmp(nm, "humn")) humn = m;

		if (sscanf(s, " %" PRId64, &m->val) == 1)
			;
		else if (sscanf(s, " %4s %c %4s", d1, &c, d2) == 3) {
			m->op = c;
			m->deps[0] = by_name(d1);
			m->deps[1] = by_name(d2);
		} else
			assert(!"bad scan");
	}

	assert(root);
	assert(humn);
}

int
main(int argc, char **argv)
{
	int64_t p1,p2;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	read_input();
	p1 = eval(root);
	root->op = '-';
	p2 = solve(root, humn, 0);

	printf("21: %" PRId64 " %" PRId64 "\n", p1,p2);
	return 0;
}
