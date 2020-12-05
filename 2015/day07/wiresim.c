#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

enum {OP_NUL, OP_MOV, OP_NOT, OP_AND, OP_OR, OP_LSH, OP_RSH};

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
	size_t nfs=0;

	while (l) {
		assert(nfs < LEN(fs));
		fs[nfs++] = strsep(&l, " ");
	}

	assert(nfs > 2);
	assert(!strcmp(fs[nfs-2], "->"));
	
	w = getwire(fs[nfs-1]);

	if (nfs == 3) {
		w->op = OP_MOV;
		setin(w, 0, fs[0]);
	} else if (nfs == 4 && !strcmp(fs[0], "NOT")) {
		w->op = OP_NOT;
		setin(w, 0, fs[1]);
	} else if (nfs == 5) {
		     if (!strcmp(fs[1], "AND"))    w->op = OP_AND;
		else if (!strcmp(fs[1], "OR"))     w->op = OP_OR;
		else if (!strcmp(fs[1], "LSHIFT")) w->op = OP_LSH;
		else if (!strcmp(fs[1], "RSHIFT")) w->op = OP_RSH;
		else assert(0 && "bad instruction");

		setin(w, 0, fs[0]);
		setin(w, 1, fs[2]);
	} else
		assert(0 && "bad instruction");
}

static void
dump(struct wire *w)
{
	if (w->op == OP_NOT)
		printf("NOT ");

	if (w->in[0])
		printf("%s ", w->in[0]->nm);
	else
		printf("%u ", (unsigned)w->imm[0]);

	switch (w->op) {
		case OP_AND: printf("AND "); break;
		case OP_OR:  printf("OR ");  break;
		case OP_LSH: printf("LSHIFT "); break;
		case OP_RSH: printf("RSHIFT "); break;
	}

	if (w->op < OP_AND)
		;
	else if (w->in[1])
		printf("%s ", w->in[1]->nm);
	else
		printf("%u ", (unsigned)w->imm[1]);

	printf("-> %s\n", w->nm);
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
