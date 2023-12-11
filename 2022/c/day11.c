#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct monkey {
	int vals[64];
	size_t nvals;
	int op_type, op_arg;
#define OP_ADD		1
#define OP_MUL		2
#define OP_SQUARE	3	/* arg unused */
	int test_arg, dest_true, dest_false;
	uint64_t ntests;
};


static char *
read_line(void)
{
	char buf[64], *p;

	p = fgets(buf, 64, stdin);
	//printf("read: %s", p);

	return p;
}

static int
find_num(char *s)
{
	while (*s && !isdigit(*s)) s++;

	return atoi(s);
}

static void UNUSED
dump(struct monkey *monkeys, size_t nmonkeys)
{
	size_t i,j;

	for (i=0; i<nmonkeys; i++) {
		printf("%zu, op=[old ", i);

		switch (monkeys[i].op_type) {
		case OP_ADD: printf("+ %3d", monkeys[i].op_arg); break;
		case OP_MUL: printf("* %3d", monkeys[i].op_arg); break;
		case OP_SQUARE: printf("* old"); break;
		default: printf("<bad op>"); break;
		}

		printf("] test=[%%%d ? %d : %d] ntests=%zu:",
		    monkeys[i].test_arg,
		    monkeys[i].dest_true,
		    monkeys[i].dest_false,
		    (size_t)monkeys[i].ntests);

		for (j=0; j < monkeys[i].nvals; j++)
			printf(" %d", monkeys[i].vals[j]);

		printf("\n");
	}
}

static size_t
read_monkeys(struct monkey *monkeys, size_t sz)
{
	struct monkey *monkey;
	size_t len=0;
	char *s, *field;

	do {
		assert(len < sz);
		monkey = &monkeys[len++];

		s = read_line(); assert(s);
		s = read_line(); assert(s);

		while ((field = strsep(&s, ","))) {
			assert(monkey->nvals < LEN(monkey->vals));
			monkey->vals[monkey->nvals++] =
			    find_num(field);
		}

		s = read_line(); assert(s);

		if (strstr(s, "old * old"))
			monkey->op_type = OP_SQUARE;
		else if (strstr(s, "old +")) {
			monkey->op_type = OP_ADD;
			monkey->op_arg = find_num(s);
		} else if (strstr(s, "old *")) {
			monkey->op_type = OP_MUL;
			monkey->op_arg = find_num(s);
		} else
			assert(!"bad op");

		monkey->test_arg = find_num(read_line());
		monkey->dest_true = find_num(read_line());
		monkey->dest_false = find_num(read_line());
	} while (read_line()); /* empty line */

	return len;
}

static void
throw_item(
    struct monkey *monkey, int64_t val,
    int div_rule, int mod_rule,
    struct monkey *monkeys, size_t nmonkeys)
{
	int dest;

	monkey->ntests++;

	switch (monkey->op_type) {
	case OP_ADD: val += monkey->op_arg; break;
	case OP_MUL: val *= monkey->op_arg; break;
	case OP_SQUARE: val *= val; break;
	default: assert(!"bad op");
	}

	val = val / div_rule % mod_rule;
	assert(val >= 0);
	assert(val < INT_MAX);

	dest = val % monkey->test_arg == 0
	    ? monkey->dest_true
	    : monkey->dest_false;

	assert(dest >= 0 && (size_t)dest < nmonkeys);
	assert(monkeys[dest].nvals < LEN(monkeys[dest].vals));

	monkeys[dest].vals[monkeys[dest].nvals++] = (int)val;
}

static uint64_t
run_part(
    struct monkey *monkeys, size_t nmonkeys,
    size_t nrounds, int div_rule, int mod_rule)
{
	size_t round, i,j;
	uint64_t top1=0, top2=0;

	for (round=0; round < nrounds; round++) {
		for (i=0; i < nmonkeys; i++) {
			for (j=0; j < monkeys[i].nvals; j++)
				throw_item(
				    &monkeys[i], monkeys[i].vals[j],
				    div_rule, mod_rule,
				    monkeys, nmonkeys);
			monkeys[i].nvals = 0;
		}

		//printf("ROUND %zu\n", round);
		//dump(monkeys, nmonkeys);
	}

	for (i=0; i < nmonkeys; i++)
		if (monkeys[i].ntests > top1) {
			top2 = top1;
			top1 = monkeys[i].ntests;
		} else if (monkeys[i].ntests > top2)
			top2 = monkeys[i].ntests;

	return top1 * top2;
}

int
main(int argc, char **argv)
{
	static struct monkey monkeys_p1[8], monkeys_p2[8];
	size_t nmonkeys, i;
	int mod=1;
	uint64_t p1,p2;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	nmonkeys = read_monkeys(monkeys_p1, LEN(monkeys_p1));
	//dump(monkeys_p1, nmonkeys);
	memcpy(monkeys_p2, monkeys_p1, sizeof(monkeys_p2));

	for (i=0; i < nmonkeys; i++)
		mod *= monkeys_p1[i].test_arg;
	//printf("mod=%d\n", mod);

	p1 = run_part(monkeys_p1, nmonkeys, 20, 3, mod);
	p2 = run_part(monkeys_p2, nmonkeys, 10000, 1, mod);

	printf("11: %" PRIu64 " %" PRIu64 "\n", p1, p2);

	return 0;
}
