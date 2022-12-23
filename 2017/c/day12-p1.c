#define _CRT_SECURE_NO_WARNINGS
#define NVERTS  2096
#define LEN(a)   (sizeof(a)/sizeof(*a))

#include <stdio.h>
#include <stdlib.h>

struct vert {
	struct vert *edges[8];
	struct vert *nextopen;
	short visited;
};

/* exits on error, returns 0 on EOF */
static size_t
parsevert(struct vert *verts, size_t nverts, size_t lineno)
{
	size_t idx, edge, nedges = 0;
	int res;
	
	if ((res = scanf(" %zu", &idx)) == EOF)
		return 0;
	if (res != 1) {
		fprintf(stderr, "%zu: expected vertex index\n", lineno);
		exit(1);
	}

	if (idx >= nverts) {
		fprintf(stderr, "%zu: index %zu is too large\n", lineno, idx);
		exit(1);
	}

	if (scanf(" <-> %zu", &edge) != 1)
		return 1;

	do {
		if (edge >= nverts) {
			fprintf(stderr, "%zu: edge index %zu is too large\n",
				lineno, edge);
			exit(1);
		}

		if (++nedges > LEN(verts[idx].edges)) {
			fprintf(stderr, "%zu: too many edges\n", lineno);
			exit(1);
		}
		
		verts[idx].edges[nedges-1] = &verts[edge];
	} while (scanf(", %zu", &edge));

	return 1;
}

/* assumes zeroed .visited and .nextopen */
static size_t
findreach(size_t target, struct vert *verts)
{
	struct vert *open, *edge;
	size_t reach = 0, i;

	open = &verts[target];	
	open->visited = 1;

	while (open) {
		printf("on %zu\n", open - verts);
		reach++;

		for (i = 0; i < LEN(open->edges) && open->edges[i]; i++) {
			edge = open->edges[i];

			if (edge->visited) {
				printf(" skip %zu\n", edge - verts);
			} else {
				printf(" queue %zu\n", edge - verts);
				edge->visited = 1; /* prevent requeueing */
				edge->nextopen = open->nextopen;
				open->nextopen = open->edges[i];	
			}
		}

		open = open->nextopen;
	}

	return reach;
}

int
main(int argc, char **argv)
{
	struct vert *verts;
	char *endptr;
	size_t lineno = 0, target;

	if (argc < 2 || !argv[1]) {
		fputs("usage: day12a TARGET <input.txt\n", stderr);
		return 1;
	}

	target = (size_t)strtol(argv[1], &endptr, 0);
	if (*endptr || target >= NVERTS) {
		printf("invalid target value: %s\n", argv[1]);
		return 1;
	}

	if (!(verts = calloc(NVERTS, sizeof(*verts)))) {
		perror(NULL);
		return 1;
	}
	
	while (parsevert(verts, NVERTS,++lineno))
		;

	printf("\ncount: %zu\n", findreach(target, verts));
	return 0;
}
