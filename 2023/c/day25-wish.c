#include "common.h"

/*
 * A Wish version of the Kernighan–Lin algorithm, but it gets stuck
 * in a local optimum for the real input and random prodding didn't
 * help.
 *
 * Ended up solving the problem in Python with NetworkX,
 * see ../python/day25.py
 */

struct node {
	char name[4];
	struct node *edges[16];
	int nedges, group;
};

static struct node nodes[1600];
static int nnodes;

static struct node *
get_node(const char *name)
{
	int i;

	for (i=0; i<nnodes; i++)
		if (!strcmp(nodes[i].name, name))
			return &nodes[i];
	
	assert(nnodes < (int)LEN(nodes));
	snprintf(nodes[i].name, sizeof(nodes[i].name), "%s", name);
	nnodes++;

	return &nodes[i];
}

int
main(int argc, char **argv)
{
	static char line[64];
	struct node *src, *dst, *max_node;
	char *tok, *rest, *p;
	int i,j, ncross=0, balance, max_balance, count;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while ((rest = fgets(line, sizeof(line), stdin))) {
		p = strchr(line, '\n'); assert(p);
		*p = '\0';

		tok = strsep(&rest, " "); assert(tok);
		p = strchr(tok, ':'); assert(p);
		*p = '\0';
		src = get_node(tok);

		while ((tok = strsep(&rest, " "))) {
			dst = get_node(tok);
			assert(src->nedges < (int)LEN(src->edges));
			assert(dst->nedges < (int)LEN(dst->edges));
			src->edges[src->nedges++] = dst;
			dst->edges[dst->nedges++] = src;
		}
	}

	for (i=0; i<nnodes; i++)
		nodes[i].group = i&1;
	
	for (i=0; i<nnodes; i++)
	for (j=0; j<nodes[i].nedges; j++)
		ncross += !nodes[i].group && nodes[i].edges[j]->group;

	while (ncross > 3) {
		max_node = NULL;
		max_balance = INT_MIN;

		for (i=0; i<nnodes; i++) {
			for (j=0, balance=0; j<nodes[i].nedges; j++)
				balance +=
				    nodes[i].group ==
				    nodes[i].edges[j]->group ? -1 : 1;
			
			if (balance > max_balance) {
				max_balance = balance;
				max_node = &nodes[i];
			}
		}

		assert(max_node);
		ncross -= max_balance;
		max_node->group = !max_node->group;

		printf(" flipping %s, crossess now %d\n",
		    max_node->name, ncross);
	}

	for (count=0, i=0; i<nnodes; i++)
		count += !nodes[i].group;
	
	printf(" %d in group 1\n", count);
	return 0;
}
