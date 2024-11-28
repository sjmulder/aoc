#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct mon {
	char name[5], op;
	struct mon *deps[2];
	int64_t val;
};

static struct mon mons[2560], *root;
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

static int64_t
eval(struct mon *m)
{
	switch (m->op) {
	case 0: return m->val;
	case '+': return eval(m->deps[0]) + eval(m->deps[1]);
	case '-': return eval(m->deps[0]) - eval(m->deps[1]);
	case '*': return eval(m->deps[0]) * eval(m->deps[1]);
	case '/': return eval(m->deps[0]) / eval(m->deps[1]);
	default: assert(!"bad op"); abort();
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
		if (!strcmp(nm, "root")) root = m;

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
}

int
main()
{
	struct mon *m,*d0,*d1;
	size_t i;

	read_input();

	printf("digraph day21 {\n");

	for (i=0; i<nm; i++) {
		m = &mons[i]; d0 = m->deps[0]; d1 = m->deps[1];

		if (!m->op)
			printf("  %s [label=\"%" PRId64 "\"]\n",
			    m->name, m->val);
		else {
			printf("  %s [label=\"%c (%" PRId64 ")\"]\n",
			    m->name, m->op, eval(m));
			printf("  %s-> %s\n", m->name, d0->name);
			printf("  %s-> %s\n", m->name, d1->name);
		}
	}

	printf("}\n");

	return 0;
}
