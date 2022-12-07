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
	int size_total;
	struct node *parent;
	struct node *children;
	struct node *next;
};

static struct node nodes[512], *cwd;
static size_t nnodes;

/* creates if not not exists */
static struct node *
get_child(char *name)
{
	struct node *node;

	assert(cwd);

	for (node = cwd->children;
	     node && strcmp(name, node->name);
	     node = node->next) ;
	
	return node;
}

static void
read_input(void)
{
	char buf[64], *s, *fields[4], *lf;
	struct node *node;
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
			node->size = atoi(fields[0]);
			node->is_dir = !strcmp(fields[0], "dir");
			snprintf(node->name, sizeof(node->name), "%s",
			    fields[1]);

			node->parent = cwd;
			node->next = cwd->children;
			cwd->children = node;
		} else if (!strcmp(fields[1], "cd")) {
			cwd =
			    !strcmp(fields[2], "/") ? &nodes[0] :
			    !strcmp(fields[2], "..") ? cwd->parent :
			    get_child(fields[2]);
			assert(cwd);
		}
	}
}


static void
update_totals(struct node *node)
{
	struct node *child;

	if (node->is_dir) {
		node->size_total = 0;
		for (child = node->children; child; child = child->next) {
			update_totals(child);
			node->size_total += child->size_total;
		}
	} else {
		node->size_total = node->size;
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
	update_totals(root);

	//dump(&nodes[0], 0);

	p2_target = 30000000 - 70000000 + root->size_total;

	for (i=0; i<nnodes; i++) {
		if (!nodes[i].is_dir)
			continue;
		if (nodes[i].size_total <= 100000)
			p1 += nodes[i].size_total;
		if (nodes[i].size_total > p2_target &&
		    nodes[i].size_total <= p2)
			p2 = nodes[i].size_total;
	}

	printf("07: %d %d\n", p1, p2);

	return 0;
}
