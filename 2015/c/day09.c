#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <err.h>

#define MAX_LOCS	8

static size_t nlocs;
static char names[MAX_LOCS][16];
static int costs[MAX_LOCS][MAX_LOCS];
static int min_cost=INT_MAX, max_cost=INT_MIN;

static int
get_idx(const char *name)
{
	size_t i;

	for (i=0; i<nlocs; i++)
		if (!strcmp(names[i], name))
			return i;

	if (nlocs > MAX_LOCS)
		errx(1, "locs overflow");

	snprintf(names[nlocs], sizeof(*names), "%s", name);
	return nlocs++;
}

static void
recur(int cost, int loc, unsigned visit_mask)
{
	size_t i;
	int new_cost, end=1;

	for (i=0; i < nlocs; i++) {
		if (visit_mask & (1<<i))
			continue;
		new_cost = cost + (loc == -1 ? 0 : costs[loc][i]);
		recur(new_cost, i, visit_mask | (1<<i));
		end = 0;
	}

	if (end) {
		if (cost < min_cost) min_cost = cost;
		if (cost > max_cost) max_cost = cost;
	}
}

int
main()
{
	char from[16], to[16];
	int from_idx, to_idx, cost;

	while (scanf("%15s to %15s = %d", from, to, &cost) == 3) {
		from_idx = get_idx(from);
		to_idx = get_idx(to);
		costs[from_idx][to_idx] = cost;
		costs[to_idx][from_idx] = cost;
	}

	recur(0, -1, 0);

	printf("09: %d %d\n", min_cost, max_cost);
	return 0;
}
