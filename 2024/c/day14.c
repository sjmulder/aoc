#include "common.h"

#define SAMPLE 0
#define GW (SAMPLE ? 11 : 101)
#define GH (SAMPLE ?  7 : 103)
#define NR 500

int
main(int argc, char **argv)
{
	static char g[GH][GW];
	static int px[NR],py[NR], vx[NR],vy[NR];

	int nr, nt, i,j, x,y;
	uint64_t order, max_order=0, acc;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (nr=0; nr<NR; nr++) {
		nt = scanf(" p=%d,%d v=%d,%d",
		    &px[nr], &py[nr],
		    &vx[nr], &vy[nr]);
		if (nt != 4)
			break;
	}

	for (i=0; i < 5*60*60; i++) {
		memset(g, 0, sizeof(g));

		for (j=0; j<nr; j++) {
			px[j] = (px[j] + vx[j] + GW) % GW;
			py[j] = (py[j] + vy[j] + GH) % GH;
			g[py[j]][px[j]] = 1;
		}

		order = 0;

		for (y=0; y<GH; y++) {
			for (acc=1, x=0; x<GW; x++)
				if (g[y][x])
					acc *= 2;
			order += acc;
		}

		for (x=0; x<GW; x++) {
			for (acc=1, y=0; y<GH; y++)
				if (g[y][x])
					acc *= 2;
			order += acc;
		}

		if (order >= max_order) {
			printf("%d (%d:%02d:%02d): order %"PRIu64"\n",
			    i, i/60/60, i/60%60, i%60, order);
			max_order = order;
		}
	}

	return 0;
}
