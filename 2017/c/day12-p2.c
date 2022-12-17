#define _CRT_SECURE_NO_WARNINGS
#define NVERTS  2096
#define LEN(a)   (sizeof(a)/sizeof(*a))

#include <stdio.h>
#include <stdlib.h>

struct vert {
	struct vert *edges[8];
	struct vert *nextopen;
	short visited;
	short color;
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

static void
colorfrom(struct vert *first, struct vert *verts, size_t nverts, short color)
{
	struct vert *open, *edge;
	size_t i;

	for (i = 0; i < nverts; i++) {
		verts[i].visited = 0;
		verts[i].nextopen = NULL;
	}

	open = first;	
	open->visited = 1;

	printf("COLOR %hd from %zu\n", color, open - verts);

	while (open) {
		printf("coloring %zu in %hd\n", open - verts, color);
		open->color = color;

		for (i = 0; i < LEN(open->edges) && open->edges[i]; i++) {
			edge = open->edges[i];

			if (edge->color) {
				printf(" skip %zu (colored)\n", edge - verts);
			} else if (edge->visited) {
				printf(" skip %zu (visited)\n", edge - verts);
			} else {
				printf(" queue %zu\n", edge - verts);
				edge->visited = 1; /* prevent requeueing */
				edge->nextopen = open->nextopen;
				open->nextopen = open->edges[i];	
			}
		}

		open = open->nextopen;
	}
}

int
main()
{
	struct vert *verts;
	size_t lineno = 0, i;
	short ncolors = 0;

	if (!(verts = calloc(NVERTS, sizeof(*verts)))) {
		perror(NULL);
		return 1;
	}
	
	while (parsevert(verts, NVERTS, ++lineno))
		;

	for (i = 0; i < NVERTS; i++) {
		/* restrict painting to verts specified in input; those
		   have at least one edge */
		if (verts[i].edges[0] && !verts[i].color)
			colorfrom(&verts[i], verts, NVERTS, ++ncolors);
	}

	printf("\nnumber of colors: %hd\n", ncolors);
	return 0;
}
