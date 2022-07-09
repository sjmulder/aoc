#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

enum {OP_NUL, OP_MOV, OP_NOT, OP_AND, OP_OR, OP_LSH, OP_RSH};

static const struct {
	char nm[7];
	int nfs;
} ops[] = {
	{}, {"", 3}, {"NOT", 4}, {"AND", 5}, {"OR", 5},
	{"LSHIFT", 5}, {"RSHIFT", 5}
};

struct wire {
	char nm[3];
	int op, flags;
#define W_CACHED 1
	struct wire *in[2];
	uint16_t imm[2], val;
};

static int verbose;
static struct wire wires[1024];
static size_t nwires = 0;

static struct wire *
getwire(char *nm)
{
	size_t i;

	assert(nm);
	assert(strlen(nm) < 3);

	for (i=0; i < nwires; i++)
		if (!strcmp(wires[i].nm, nm))
			return &wires[i];

	assert(nwires < LEN(wires));
	memcpy(wires[nwires].nm, nm, 3);
	return &wires[nwires++];
}

static void
setin(struct wire *w, int i, char *s)
{
	if (s[0] >= '0' && s[0] <= '9')
		w->imm[i] = atoi(s);
	else
		w->in[i] = getwire(s);
}

static char *
instr(struct wire *w, int i)
{
	static char buf[16];

	if (w->in[i])
		snprintf(buf, 16, "%s", w->in[i]->nm);
	else
		snprintf(buf, 16, "%u", (unsigned)w->imm[i]);

	return buf;
}

static uint16_t
solve(struct wire *w)
{
	uint16_t l,r;

	assert(w);
	assert(w->op);

	if (w->flags & W_CACHED)
		return w->val;
	if (verbose>1)
		fprintf(stderr, "+%s\n", w->nm);

	l = w->in[0] ? solve(w->in[0]) : w->imm[0];
	r = w->in[1] ? solve(w->in[1]) : w->imm[1];

	switch (w->op) {
		case OP_MOV: w->val = l; break;
		case OP_NOT: w->val = ~l; break;
		case OP_AND: w->val = l & r; break;
		case OP_OR:  w->val = l | r; break;
		case OP_LSH: w->val = l << r; break;
		case OP_RSH: w->val = l >> r; break;
		default: assert(0 && "bad op");
	}

	if (verbose>1)
		fprintf(stderr, "-%s=%u\n", w->nm, (unsigned)w->val);

	w->flags |= W_CACHED;
	return w->val;
}

static void
parse(char *l)
{
	struct wire *w;
	char *fs[5];
	int nfs=0, i;

	while (l) {
		assert(nfs < (int)LEN(fs));
		fs[nfs++] = strsep(&l, " ");
	}

	assert(nfs > 2);
	assert(!strcmp(fs[nfs-2], "->"));
	
	w = getwire(fs[nfs-1]);
	w->op = 0;

	for (i=1; !w->op && i < (int)LEN(ops); i++) {
		if (nfs != ops[i].nfs)
			;
		else if (nfs == 3) {
			w->op = i;
			setin(w, 0, fs[0]);
		} else if (nfs == 4 && !strcmp(fs[0], ops[i].nm)) {
			w->op = i;
			setin(w, 0, fs[1]);
		} else if (nfs == 5 && !strcmp(fs[1], ops[i].nm)) {
			w->op = i;
			setin(w, 0, fs[0]);
			setin(w, 1, fs[2]);
		}
	}

	assert(w->op && "bad op");
}

static void
dump(struct wire *w)
{
	switch (ops[w->op].nfs) {
	case 3: printf("%s", instr(w, 0)); break;
	case 4: printf("%s %s", ops[w->op].nm, instr(w, 0)); break;
	case 5: printf("%s %s %s", instr(w, 0), ops[w->op].nm,
	    instr(w, 1)); break;
	default: printf("???"); break;
	}

	printf(" -> %s\n", w->nm);
}

int
main(int argc, char **argv)
{
	char buf[128], *lf;
	int c, dumpfl=0;
	size_t i;

	while ((c = getopt(argc, argv, "dv")) != -1) {
		switch (c) {
		case 'd': dumpfl = 1; break;
		case 'v': verbose++; break;
		}
	}

	argc -= optind;
	argv += optind;

	while (fgets(buf, 128, stdin)) {
		if ((lf = strchr(buf, '\n')))
			*lf = '\0';
		parse(buf);
	}

	if (dumpfl) {
		for (i=0; i<nwires; i++)
			dump(&wires[i]);
		return 0;
	}

	if (argc) {
		for (i=0; (int)i<argc; i++)
			printf("%u\n", (unsigned)solve(
			    getwire(argv[i])));
	} else {
		for (i=0; i<nwires; i++)
			printf("%s: %u\n", wires[i].nm,
			    solve(&wires[i]));
	}
}
