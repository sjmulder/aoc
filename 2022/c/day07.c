#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

/*
 * This version of the solution assumes things about the input, e.g.:
 *  - Every command not 'cd' is 'ls'.
 *  - 'cd' takes no path, just a name.
 *  - Files and directories are listed exactly once.
 *  - Directories are listed before entering them.
 *
 * The original solution without these assumptions is day07-orig.c
 */

struct node {
	char name[64];
	int size, is_dir;
	struct node *parent, *children, *next;
};

int
main(int argc, char **argv)
{
	static struct node nodes[512];
	static char buf[64];

	struct node *cwd, *node, *n;
	char *fields[4], *s, *lf;
	size_t nnodes=1, nf=0, i;
	int p1=0, p2=INT_MAX, p2_target;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);
	
	nodes[0].is_dir = 1;
	cwd = &nodes[0];

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

	p2_target = 30000000 - 70000000 + nodes[0].size;

	for (i=0; i<nnodes; i++) {
		if (!nodes[i].is_dir)
			continue;
		if (nodes[i].size <= 100000)
			p1 += nodes[i].size;
		if (nodes[i].size > p2_target && nodes[i].size <= p2)
			p2 = nodes[i].size;
	}

	printf("07: %d %d\n", p1, p2);

	return 0;
}
