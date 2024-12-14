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

	int nr, ntok, sec,i, x,y, run;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (nr=0; nr<NR; nr++) {
		ntok = scanf(" p=%d,%d v=%d,%d",
		    &px[nr], &py[nr],
		    &vx[nr], &vy[nr]);
		if (ntok != 4)
			break;
	}

	for (sec=1; ; sec++) {
		memset(g, 0, sizeof(g));

		for (i=0; i<nr; i++) {
			px[i] = (px[i] + vx[i] + GW) % GW;
			py[i] = (py[i] + vy[i] + GH) % GH;

			g[py[i]][px[i]] = 1;
		}

		for (y=0; y<GH; y++)
		for (x=0, run=0; x<GW; x++)
			if (!g[y][x])
				run = 0;
			else if (++run >= 10)
				goto found_p2;
	}

found_p2:
	printf("14: __ %d\n", sec);
	return 0;
}
