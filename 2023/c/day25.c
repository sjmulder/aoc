#include "common.h"

struct node {
	char name[4];
	struct node *edges[16];
	int nedges, label;
};

struct count { int key, count; };
struct cmap { struct count *counts; size_t len, cap; };

static struct node nodes[1600];
static int nnodes;

static void
cmap_init(struct cmap *map, struct count *counts, size_t cap)
{
	map->counts = counts;
	map->len = 0;
	map->cap = cap;
}

static int
cmap_get(struct cmap *map, int key)
{
	size_t i;

	for (i=0; i < map->len; i++)
		if (map->counts[i].key == key)
			return map->counts[i].count;
	
	return 0;
}

static int
cmap_inc(struct cmap *map, int key)
{
	size_t i;

	for (i=0; i < map->len; i++)
		if (map->counts[i].key == key)
			break;
	
	if (i == map->len) {
		assert(i < map->cap);
		map->len++;
		map->counts[i].key = key;
		map->counts[i].count = 0;
	}

	return ++map->counts[i].count;
}

static int
cmap_dec(struct cmap *map, int key)
{
	size_t i;

	for (i=0; i < map->len; i++)
		if (map->counts[i].key == key)
			return --map->counts[i].count;
	
	assert(i != map->len);
	return 0;
}

static struct count *
cmap_max(
    struct cmap *map,
    int (*cmp)(const struct count *a, const struct count *b, void *ctx),
    void *ctx)
{
	struct count *max;
	size_t i;
	int larger;

	if (!map->len)
		return NULL;

	max = &map->counts[0];

	for (i=1; i < map->len; i++) {
		larger = cmp
		    ? cmp(&map->counts[i], max, ctx) > 0
		    : map->counts[i].count > max->count;
		if (larger)
			max = &map->counts[i];
	}

	return max;
}

static void
cmap_dump(struct cmap *map)
{
	size_t i;

	for (i=0; i < map->len; i++)
		printf("  %d: %d\n",
		    map->counts[i].key,
		    map->counts[i].count);
}

static int
cmp_label_counts(const struct count *a, const struct count *b,
    void *ctx)
{
	const struct count *ca=a, *cb=b;
	struct cmap *global_counts=ctx;

	if (ca->count < cb->count) return -1;
	if (ca->count < cb->count) return -1;
	if (!global_counts) return -1;

	return
	    cmap_get(global_counts, ca->key) -
	    cmap_get(global_counts, cb->key);
}

static void
dump_labels(void)
{
	int i;

	struct count counts[LEN(nodes)];
	struct cmap cmap;

	cmap_init(&cmap, counts, LEN(counts));

	for (i=0; i<nnodes; i++)
		cmap_inc(&cmap, nodes[i].label);
	
	cmap_dump(&cmap);
}

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

static void
parse(void)
{
	static char line[64];
	struct node *src, *dst;
	char *tok, *rest, *p;

	while ((rest = fgets(line, sizeof(line), stdin))) {
		p = strchr(line, '\n'); assert(p); *p = '\0';

		tok = strsep(&rest, " "); assert(tok);
		p = strchr(tok, ':'); assert(p); *p = '\0';
		src = get_node(tok);

		while ((tok = strsep(&rest, " "))) {
			dst = get_node(tok);
			assert(src->nedges < (int)LEN(src->edges));
			assert(dst->nedges < (int)LEN(dst->edges));
			src->edges[src->nedges++] = dst;
			dst->edges[dst->nedges++] = src;
		}
	}
}

static void
label_propagation(void)
{
	static struct count global_counts[LEN(nodes)];
	static struct count counts[16];
	struct cmap global_map, map;
	struct count *max;
	int nchange, i,j;

	cmap_init(&global_map, global_counts, LEN(global_counts));

	for (i=0; i<nnodes; i++) {
		nodes[i].label = i;
		cmap_inc(&global_map, i);
	}
	
	for (nchange=1; nchange; )
	for (nchange=0, i=0; i<nnodes; i++) {
		cmap_init(&map, counts, LEN(counts));

		for (j=0; j < nodes[i].nedges; j++)
			cmap_inc(&map, nodes[i].edges[j]->label);

		max = cmap_max(&map, cmp_label_counts, &global_map);
		assert(max);

		printf(" %s (%d)\n", nodes[i].name, nodes[i].label);
		cmap_dump(&map);

		//printf(" max count: %d, label: %d\n",
		//    max->count, max->key);

		if (nodes[i].label != max->key) {
			printf(" switching %s to %d\n",
			    nodes[i].name, max->key);
			cmap_dec(&global_map, nodes[i].label);
			cmap_inc(&global_map, max->key);
			nodes[i].label = max->key;
			nchange++;
		}
	}
}

int
main(int argc, char **argv)
{
	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	parse();
	label_propagation();
	dump_labels();

	return 0;
}
