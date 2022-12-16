#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

struct valve;
struct valve { char name[3]; int rate, is_open; };

static struct valve valves[64];
static size_t nvalves;
static int dists[64][64];	/* min. distances between valves */

int min(int a, int b) { return a<b ? a : b; }
int max(int a, int b) { return a>b ? a : b; }

/* creates if not exists */
static int
lookup_name(const char name[3])
{
	size_t i;

	for (i=0; i<nvalves; i++)
		if (!strcmp(name, valves[i].name))
			return i;

	assert(i+1 < LEN(valves));
	memcpy(valves[nvalves++].name, name, 3);

	return i;
}

static int
max_val(
    size_t pos1, int time1,	/* actor 1 pos and time left */
    size_t pos2, int time2)	/* actor 2 */
{
	size_t i;
	int best=0, rest, val;

	if (time1 <= 1 && time2 <= 1)
		return 0;

	for (i=0; i<nvalves; i++) {
		if (!valves[i].rate) continue;
		if (valves[i].is_open) continue;

		valves[i].is_open = 1;

		if (time1 >= time2) {
			rest = time1 - dists[pos1][i] -1;
			val = valves[i].rate * rest +
			      max_val(i, rest, pos2, time2);
			if (val > best)
				best = val;
		}

		/* extra check to prevent identical work */
		if (time2 > time1 || (time1 == time2 && pos1 != pos2)) {
			rest = time2 - dists[pos2][i] -1;
			val = valves[i].rate * rest +
			      max_val(pos1, time1, i, rest);
			if (val > best)
				best = val;
		}

		valves[i].is_open = 0;
	}

	return best;
}

int
main()
{
	char buf[128], name[5], exits[5][3];
	int p1,p2, n, rate, idx;
	size_t i,j,k;

	memset(dists, 0x0f, sizeof(dists)); /* large numbers */

	while (fgets(buf, sizeof(buf), stdin)) {
		memset(exits, 0, sizeof(exits));

		n = sscanf(buf, strstr(buf, "tunnels")
		    ? "Valve %2s has flow rate=%d; "
		      "tunnels lead to valves %2s, %2s, %2s, %2s, %2s"
		    : "Valve %2s has flow rate=%d; "
		      "tunnel leads to valve %2s",
		    name, &rate,
		    exits[0], exits[1], exits[2], exits[3], exits[4]);

		idx = lookup_name(name);
		valves[idx].rate = rate;

		for (i=2; i < (size_t)n; i++)
			dists[idx][lookup_name(exits[i-2])] = 1;
	}

	/* Floyd-Warshall algorithm - all-pair shortest paths */
	for (k=0; k < nvalves; k++)
	for (i=0; i < nvalves; i++)
	for (j=0; j < nvalves; j++)
		dists[i][j] = min(dists[i][j], dists[i][k]+dists[k][j]);

	idx = lookup_name("AA");
	p1 = max_val(idx, 30, 0, 0);
	p2 = max_val(idx, 26, idx, 26),

	printf("16: %d %d\n", p1, p2);
	return 0;
}
