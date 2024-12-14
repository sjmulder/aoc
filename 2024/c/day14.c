#include "common.h"

#define SAMPLE 0
#define GW (SAMPLE ? 11 : 101)
#define GH (SAMPLE ?  7 : 103)
#define NR 501

int
main(int argc, char **argv)
{
	static char g[GH][GW];
	static int px[NR],py[NR], vx[NR],vy[NR];

	int p1=0, n=0, sec, i, x,y, q[4]={}, run;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (; scanf(" p=%d,%d v=%d,%d", px+n,py+n, vx+n,vy+n)==4; n++)
		assert(n+1 < NR);

	for (sec=1; !SAMPLE || sec <= 100; sec++) {
		memset(g, 0, sizeof(g));
		memset(q, 0, sizeof(q));

		for (i=0; i<n; i++) {
			px[i] = (px[i] + vx[i] + GW) % GW;
			py[i] = (py[i] + vy[i] + GH) % GH;

			g[py[i]][px[i]] = 1;

			if (sec == 100) {
				if (px[i] < GW/2) {
					if (py[i] < GH/2) q[0]++; else
					if (py[i] > GH/2) q[1]++;
				} else if (px[i] > GW/2) {
					if (py[i] < GH/2) q[2]++; else
					if (py[i] > GH/2) q[3]++;
				}
			}
		}

		if (sec == 100)
			p1 = q[0]*q[1]*q[2]*q[3];

		for (y=0; y<GH; y++)
		for (x=0, run=0; x<GW; x++)
			if (!g[y][x])
				run = 0;
			else if (++run >= 10)
				goto found_p2;
	}

found_p2:
	printf("14: %d %d\n", p1, sec);
	return 0;
}
