#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define UNUSED	__attribute__((unused))
#define LEN(a)	(sizeof(a)/sizeof(*(a)))

/*
 * This is my original solution. It does not make the assumptions
 * day07.c makes and it supports paths for 'cd', including e.g.
 * 'cd /foo/../bar/baz'
 */

struct node {
	int flags;
#define NODE_DIR	1
	char name[64];
	int size;
	int size_total;
	struct node *parent;
	struct node *children;
	struct node *next;
};

static struct node nodes[512], *cwd;
static size_t nnodes;

static UNUSED void
dump(struct node *node, int depth)
{
	int i=0;
	struct node *child;

	if (node->flags & NODE_DIR)
		printf("     dir ");
	else
		printf("%8d ", node->size);
	
	for (i=0; i<depth; i++)
		printf("| ");
	printf("%s\n", node->name);

	for (child = node->children; child; child = child->next)
		dump(child, depth+1);
}

static int
split_fields(char *s, char **fields, int sz)
{
	int num=0;

	while (num < sz && (fields[num] = strsep(&s, " ")))
		num++;
	
	return num;
}

static struct node *
get_child(char *name, int flags)
{
#define CHILD_CREATE	1

	struct node **nodep, *node;

	assert(cwd);

	nodep = &cwd->children;

	while (*nodep && strcmp(name, (*nodep)->name))
		nodep = &(*nodep)->next;
	
	if (!*nodep && (flags & CHILD_CREATE)) {
		assert(nnodes+1 < sizeof(nodes));
		node = &nodes[nnodes++];
		node->parent = cwd;
		snprintf(node->name, sizeof(node->name), "%s", name);
		*nodep = node;
	}
	
	return *nodep;
}

static void
traverse(char *path, int flags)
{
	char *name;

	while (path && path[0]) {
		name = strsep(&path, "/");

		if (!strcmp(name, "")) {
			cwd = &nodes[0];
			path = path+1;
		} else if (!strcmp(name, "..")) {
			assert(cwd->parent);
			cwd = cwd->parent;
		} else if (strcmp(name, ".") != 0) {
			cwd = get_child(name, flags);
			cwd->flags = NODE_DIR;
		}
	}
}

static void
read_input(void)
{
	char buf[64], *fields[4], *lf;
	struct node *node;
	int nfields;
	
	assert(nnodes);
	assert(cwd);

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((lf = strchr(buf, '\n')))
			*lf = '\0';

		nfields = split_fields(buf, fields, 4);
		assert(nfields > 0);

		if (!strcmp(fields[0], "$")) {
			assert(nfields >= 2);
			
			if (!strcmp(fields[1], "cd")) {
				assert(nfields == 3);
				traverse(fields[2], CHILD_CREATE);
			} else if (!strcmp(fields[1], "ls")) {
				assert(nfields == 2);
			} else {
				assert(!"bad command");
			}
		} else {
			assert(nfields == 2);

			node = get_child(fields[1], CHILD_CREATE);
			node->size = atoi(fields[0]);
			node->flags = !strcmp(fields[0], "dir")
			    ? NODE_DIR : 0;
		}
	}
}


static void
update_totals(struct node *node)
{
	struct node *child;

	if (node->flags & NODE_DIR) {
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
	root->flags = NODE_DIR;
	snprintf(root->name, sizeof(root->name), "ROOT");
	cwd = root;

	read_input();
	update_totals(root);

	//dump(&nodes[0], 0);

	p2_target = 30000000 - 70000000 + root->size_total;

	for (i=0; i<nnodes; i++) {
		if (!(nodes[i].flags & NODE_DIR))
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
