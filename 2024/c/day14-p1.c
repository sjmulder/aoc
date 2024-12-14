#include "common.h"

#define SAMPLE 0
#define GW (SAMPLE ? 11 : 101)
#define GH (SAMPLE ?  7 : 103)

int
main(int argc, char **argv)
{
	int q[4]={}, px,py, vx,vy, x,y;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (scanf(" p=%d,%d v=%d,%d", &px,&py, &vx,&vy) == 4) {
		x = (px + 100*vx) % GW; x += (x<0)*GW;
		y = (py + 100*vy) % GH; y += (y<0)*GH;

		if (x < GW/2) {
			if (y < GH/2) q[0]++; else
			if (y > GH/2) q[1]++;
		} else if (x > GW/2) {
			if (y < GH/2) q[2]++; else
			if (y > GH/2) q[3]++;
		}
	}

	printf("14: %d\n", q[0]*q[1]*q[2]*q[3]);
	return 0;
}
