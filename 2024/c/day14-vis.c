#include "common.h"
#include "vis.h"

#define SAMPLE 0
#define GW (SAMPLE ? 11 : 101)
#define GH (SAMPLE ?  7 : 103)
#define NR 500
#define CELLZ 4

int
main(int argc, char **argv)
{
	static int px[NR],py[NR], vx[NR],vy[NR];
	struct vis vis;
	int nr, nt, i,j;
	uint8_t r,g,b;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (nr=0; nr<NR; nr++) {
		nt = scanf(" p=%d,%d v=%d,%d",
		    &px[nr], &py[nr],
		    &vx[nr], &vy[nr]);
		if (nt != 4)
			break;
	}

	vis_begin(&vis, "day14.mp4", 60, GW*CELLZ, GH*CELLZ);

	for (i=0; i < 5*60*60; i++) {
		vis_fill(&vis, 0,0, GW*CELLZ, GH*CELLZ, 0,0,0);

		for (j=0; j<nr; j++) {
			lerp_rainbow((float)j / nr, &r,&g,&b);
			px[j] = (px[j] + vx[j] + GW) % GW;
			py[j] = (py[j] + vy[j] + GH) % GH;
			vis_fill(&vis, px[j]*CELLZ, py[j]*CELLZ,
			    CELLZ, CELLZ, r,g,b);
		}

		vis_emit(&vis, 1);
	}

	vis_end(&vis);
	return 0;
}
