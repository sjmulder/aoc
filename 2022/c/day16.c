#include "common.h"

static char names[64][3];
static int flows[64], is_open[64], dists[64][64];
static size_t nvalves;

int min(int a, int b) { return a<b ? a : b; }
int max(int a, int b) { return a>b ? a : b; }

/* creates if not exists */
static size_t
lookup_name(const char name[3])
{
	size_t i;

	for (i=0; i<nvalves; i++)
		if (!strcmp(name, names[i]))
			return i;

	assert(i+1 < 64);
	memcpy(names[nvalves++], name, 3);
	return i;
}

static int
max_val(
    size_t pos1, int time1,	/* actor 1 pos and time left */
    size_t pos2, int time2)	/* actor 2 */
{
	size_t i;
	int best=0, rest;

	if (time1 <= 1 && time2 <= 1)
		return 0;

	for (i=0; i<nvalves; i++) {
		if (!flows[i]) continue;
		if (is_open[i]) continue;

		is_open[i] = 1;

		if (time1 >= time2) {
			rest = time1 - dists[pos1][i] -1;
			best = max(best,
			    flows[i] * rest +
			    max_val(i, rest, pos2, time2));
		}

		/* extra check to prevent identical work */
		if (time2 > time1 || (time1 == time2 && pos1 != pos2)) {
			rest = time2 - dists[pos2][i] -1;
			best = max(best,
			    flows[i] * rest +
			    max_val(pos1, time1, i, rest));
		}

		is_open[i] = 0;
	}

	return best;
}

int
main(int argc, char **argv)
{
	char buf[128], name[5], exits[5][3];
	int p1,p2, n, rate, idx;
	size_t i,j,k;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	memset(dists, 0x0f, sizeof(dists)); /* large numbers */

	while (fgets(buf, sizeof(buf), stdin)) {
		n = sscanf(buf, strstr(buf, "tunnels")
		    ? "Valve %2s has flow rate=%d; "
		      "tunnels lead to valves %2s, %2s, %2s, %2s, %2s"
		    : "Valve %2s has flow rate=%d; "
		      "tunnel leads to valve %2s",
		    name, &rate,
		    exits[0], exits[1], exits[2], exits[3], exits[4]);

		idx = (int)lookup_name(name);
		flows[idx] = rate;
		for (i=2; i < (size_t)n; i++)
			dists[idx][lookup_name(exits[i-2])] = 1;
	}

	/* Floyd-Warshall algorithm - all-pair shortest paths */
	for (k=0; k < nvalves; k++)
	for (i=0; i < nvalves; i++)
	for (j=0; j < nvalves; j++)
		dists[i][j] = min(dists[i][j], dists[i][k]+dists[k][j]);

	idx = (int)lookup_name("AA");
	p1 = max_val(idx, 30, 0, 0);
	p2 = max_val(idx, 26, idx, 26),

	printf("16: %d %d\n", p1, p2);
	return 0;
}
