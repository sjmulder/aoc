#include "common.h"

#define SAMPLE 0
#define GW (SAMPLE ? 11 : 101)
#define GH (SAMPLE ?  7 : 103)
#define NR 500

int
main(int argc, char **argv)
{
	static int px[NR],py[NR], vx[NR],vy[NR];
	int p1=0, nr, ntok, sec, i, q[4]={};

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
		memset(q, 0, sizeof(q));

		for (i=0; i<nr; i++) {
			px[i] = (px[i] + vx[i] + GW) % GW;
			py[i] = (py[i] + vy[i] + GH) % GH;

			if (px[i] < GW/2) {
				if (py[i] < GH/2) q[0]++; else
				if (py[i] > GH/2) q[1]++;
			} else if (px[i] > GW/2) {
				if (py[i] < GH/2) q[2]++; else
				if (py[i] > GH/2) q[3]++;
			}
		}

		if (sec == 100)
			p1 = q[0]*q[1]*q[2]*q[3];

		/* robots cosplaying a tree in the corner? */
		for (i=0; i<4; i++)
			if (q[i] > nr/2)
				goto found_p2;
	}

found_p2:
	printf("14: %d %d\n", p1, sec);
	return 0;
}
