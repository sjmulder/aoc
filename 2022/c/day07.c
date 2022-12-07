#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct node {
	char name[64];
	int is_dir;
	int size;
	struct node *parent;
	struct node *children;
	struct node *next;
};

static struct node nodes[512], *cwd;
static size_t nnodes;

static void
read_input(void)
{
	char buf[64], *s, *fields[4], *lf;
	struct node *node, *n;
	size_t nf=0;
	
	assert(nnodes);
	assert(cwd);

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((lf = strchr(buf, '\n')))
			*lf = '\0';
		for (nf=0, s=buf;
		     nf < LEN(fields) && (fields[nf] = strsep(&s, " "));
		     nf++) ;

		if (strcmp(fields[0], "$") != 0) {
			assert(nf == 2);
			assert(nnodes+1 < sizeof(nodes));

			node = &nodes[nnodes++];
			node->parent = cwd;
			node->next = cwd->children;
			node->size = atoi(fields[0]);
			node->is_dir = !strcmp(fields[0], "dir");
			snprintf(node->name, sizeof(node->name), "%s",
			    fields[1]);

			cwd->children = node;

			for (n = node->parent; n; n = n->parent)
				n->size += node->size;
		} else if (!strcmp(fields[1], "cd")) {
			if (!strcmp(fields[2], "/"))
				cwd = &nodes[0];
			else if (!strcmp(fields[2], ".."))
				cwd = cwd->parent;
			else {
				for (cwd = cwd->children;
				     cwd && strcmp(fields[2], cwd->name);
				     cwd = cwd->next) ;
			}
			assert(cwd);
		}
	}
}

int
main()
{
	struct node *root;
	int p1=0, p2=INT_MAX, p2_target;
	size_t i;

	root = &nodes[nnodes++];
	root->is_dir = 1;
	snprintf(root->name, sizeof(root->name), "ROOT");
	cwd = root;

	read_input();

	//dump(&nodes[0], 0);

	p2_target = 30000000 - 70000000 + root->size;

	for (i=0; i<nnodes; i++) {
		if (!nodes[i].is_dir)
			continue;
		if (nodes[i].size <= 100000)
			p1 += nodes[i].size;
		if (nodes[i].size > p2_target &&
		    nodes[i].size <= p2)
			p2 = nodes[i].size;
	}

	printf("07: %d %d\n", p1, p2);

	return 0;
}
