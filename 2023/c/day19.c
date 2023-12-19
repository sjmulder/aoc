#include "common.h"

#define NAMESZ	600

enum { EXPR_NONE, EXPR_LT, EXPR_GT, EXPR_CALL };

struct expr { int type, var, imm, next; };
struct partrange { int min[4], max[4]; };

static char names[NAMESZ][4];
static struct expr flows[NAMESZ][5];
static int nnames;
static int accept_id, reject_id, in_id;

static int64_t
eval(int id, struct partrange p)
{
	struct partrange q;
	struct expr *e;
	int64_t acc=0;
	int i;

	assert(id >= 0);
	assert(id < (int)LEN(flows));

	if (id == reject_id ||
	    p.min[0] > p.max[0] || p.min[1] > p.max[1] ||
	    p.min[2] > p.max[2] || p.min[3] > p.max[3])
	    	return 0;

	if (id == accept_id)
		return (int64_t)
		    (p.max[0] -p.min[0] +1) * (p.max[1] -p.min[1] +1) *
		    (p.max[2] -p.min[2] +1) * (p.max[3] -p.min[3] +1);

	for (i=0; i < (int)LEN(*flows); i++)
		switch ((e = &flows[id][i])->type) {
		case EXPR_LT:
			q = p;
			q.max[e->var] = MIN(q.max[e->var], e->imm-1);
			p.min[e->var] = MAX(p.min[e->var], e->imm);
			acc += eval(e->next, q);
			break;
		case EXPR_GT:
			q = p;
			q.min[e->var] = MAX(q.min[e->var], e->imm+1);
			p.max[e->var] = MIN(p.max[e->var], e->imm);
			acc += eval(e->next, q);
			break;
		case EXPR_CALL:
			acc += eval(e->next, p);
			return acc;
		default:
			assert(!"bad flow");
		}
	
	assert(!"out of exprs");
}

static int
to_id(const char *name)
{
	int i;

	for (i=0; i<nnames; i++)
		if (!strcmp(names[i], name))
			return i;

	assert(nnames < (int)LEN(names));
	snprintf(names[nnames], sizeof(*names), "%s", name);
	return nnames++;
}

static int
to_var(const char *name)
{
	if (name[0] == 'x') return 0; else
	if (name[0] == 'm') return 1; else
	if (name[0] == 'a') return 2; else
	if (name[0] == 's') return 3; else assert(!"bad var");
}

static struct expr
parse_expr(char *s)	/* destructive on s */
{
	struct expr e;

	if (strchr(s, '<')) {
		e.type = EXPR_LT;
		e.var = to_var(strsep(&s, "<"));
		e.imm = atoi(strsep(&s, ":"));
		e.next = to_id(s);
	} else if (strchr(s, '>')) {
		e.type = EXPR_GT;
		e.var = to_var(strsep(&s, ">"));
		e.imm = atoi(strsep(&s, ":"));
		e.next = to_id(s);
	} else {
		e.type = EXPR_CALL;
		e.next = to_id(s);
	}

	return e;
}

int
main(int argc, char **argv)
{
	static char line[64];
	struct partrange pr;
	char *rest, *elem, *p;
	int64_t p1=0, p2;
	int id, var, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	accept_id = to_id("A");
	reject_id = to_id("R");
	in_id = to_id("in");

	while ((rest = fgets(line, sizeof(line), stdin))) {
		if (!line[0] || line[0] == '\n')
			break;
		id = to_id(strsep(&rest, "{"));

		for (i=0; (elem = strsep(&rest, ",")); i++) {
			assert(i < (int)LEN(*flows));
			if ((p = strchr(elem, '\n'))) *p = '\0';
			if ((p = strchr(elem, '}')))  *p = '\0';
			flows[id][i] = parse_expr(elem);
		}
	}

	while ((rest = fgets(line, sizeof(line), stdin))) {
		assert(rest[0] == '{');
		rest++;
		if ((p = strchr(rest, '\n'))) *p = '\0';
		if ((p = strchr(rest, '}')))  *p = '\0';

		memset(&pr, 0, sizeof(pr));

		while ((elem = strsep(&rest, ","))) {
			var = to_var(strsep(&elem, "="));
			pr.min[var] = pr.max[var] = atoi(elem);
		}

		if (eval(in_id, pr))
			p1 += pr.min[0] + pr.min[1] +
			      pr.min[2] + pr.min[3];
	}

	for (i=0; i<4; i++)
		{ pr.min[i] = 1; pr.max[i] = 4000; }

	p2 = eval(in_id, pr);

	printf("19: %"PRId64" %"PRId64"\n", p1, p2);
	return 0;
}
