#include "common.h"

struct stone { int64_t px,py,pz, vx,vy,vz; };

static struct stone stones[320];
static int ns;

/* returns 1 if lines intersect; pos goes into ix/iy */
static int
intersect2d(
    double px1, double py1, double vx1, double vy1,
    double px2, double py2, double vx2, double vy2)
{
	double d1,d2, ix,iy;

	if (vx1*vy2 == vx2*vy1)
		return 0;	/* parallel */

	d1 = vy1 / vx1;		/* slope */
	d2 = vy2 / vx2;
	ix = (py2 - px2*d2 - py1 + px1*d1) / (d1 - d2);
	iy = py1 - px1*d1 + (ix)*d1;

	if (((ix - px1) / vx1 < 0) || ((ix - px2) / vx2 < 0))
		return 0;	/* in past */

	if (ix < 200000000000000.0 || ix > 400000000000000.0 ||
	    iy < 200000000000000.0 || iy > 400000000000000.0)
		return 0;	/* oob */

	return 1;
}

int
main(int argc, char **argv)
{
	int p1=0, ntok, i,j;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (1) {
		assert(ns < (int)LEN(stones));
		ntok = scanf(
		    " %"SCNd64", %"SCNd64", %"SCNd64" @ "
		    " %"SCNd64", %"SCNd64", %"SCNd64,
		    &stones[ns].px, &stones[ns].py, &stones[ns].pz,
		    &stones[ns].vx, &stones[ns].vy, &stones[ns].vz);
		if (ntok != 6) break;
		ns++;
	}

	for (i=0; i<ns-1; i++)
	for (j=i+1; j<ns; j++)
		p1 += intersect2d(
		    (double)stones[i].px, (double)stones[i].py,
		    (double)stones[i].vx, (double)stones[i].vy,
		    (double)stones[j].px, (double)stones[j].py,
		    (double)stones[j].vx, (double)stones[j].vy);

	printf("23: %d <TODO>\n", p1);
	return 0;
}
