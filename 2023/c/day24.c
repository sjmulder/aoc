#include "common.h"

/*
 * For part 2, we first try to find a match on the X/Y plane using
 * brute force, then on the X/Z plane (with fixed X).
 *
 * To find the match,the time aspect is neutralized by subtracting the
 * stone's velocity from each hailstone, i.e. switching to the stone's
 * reference frame. Then all hailstones will collide on one point.
 *
 * Hence we test each velocity candidate by seeing if (a sample of)
 * hailstones share an intersection point. That intersection point
 * is our throw position.
 */

struct stone { double pos[3], vel[3]; };

static struct stone stones[320];
static int ns;

/* returns 1 if lines intersect; pos goes into ix/iy */
static int
intersect2d(
    double px1, double py1, double vx1, double vy1,
    double px2, double py2, double vx2, double vy2,
    double *ix, double *iy)
{
	double d1,d2;

	if (vx1*vy2 == vx2*vy1)
		return 0;	/* parallel */

	if (!vx1) {		/* line 1 vertical */
		*ix = 0;
		*iy = py2 - px2*vy2/vx2;
		return 1;
	}

	if (!vx2) {		/* line 2 vertical */
		*ix = 0;
		*iy = py1 - px1*vy1/vx1;
		return 1;
	}

	d1 = vy1 / vx1;		/* slope */
	d2 = vy2 / vx2;
	*ix = (py2 - px2*d2 - py1 + px1*d1) / (d1 - d2);
	*iy = py1 - px1*d1 + (*ix)*d1;

	if (((*ix - px1) / vx1 < 0) || ((*ix - px2) / vx2 < 0))
		return 0;	/* in past */

	return 1;
}

static int
part1(void)
{
	int ans=0, i,j;
	double ix,iy;

	for (i=0; i<ns-1; i++)
	for (j=i+1; j<ns; j++)
		ans += intersect2d(
		    stones[i].pos[0], stones[i].pos[1],
		    stones[i].vel[0], stones[i].vel[1],
		    stones[j].pos[0], stones[j].pos[1],
		    stones[j].vel[0], stones[j].vel[1],
		    &ix, &iy) &&
		    ix > 200000000000000.0 && ix < 400000000000000.0 &&
		    iy > 200000000000000.0 && iy < 400000000000000.0;

	return ans;
}

/* 2D velocity test for part 2 */
static int
try_velocity(int vx, int vy, int dim1, int dim2,
    double *ixp, double *iyp)
{
	int nhits=0, hit, i;
	double ix=0,iy=0, ix_prev,iy_prev;

	if (!vx && !vy)
		return 0;

	ix_prev = 0.0;
	iy_prev = 0.0;

#define SAMPLE 20
	for (i=1; i < MIN(ns, SAMPLE); i++) {
		hit = intersect2d(
		    stones[0].pos[dim1],
		    stones[0].pos[dim2],
		    stones[0].vel[dim1] - vx,
		    stones[0].vel[dim2] - vy,
		    stones[i].pos[dim1],
		    stones[i].pos[dim2],
		    stones[i].vel[dim1] - vx,
		    stones[i].vel[dim2] - vy,
		    &ix, &iy);

		if (!hit)
			continue; /* lines may be same, ignore */

		if ((ix_prev && fabs(ix - ix_prev) > 1.0) ||
		    (iy_prev && fabs(iy - iy_prev) > 1.0))
			break;	/* no match */

		nhits++;
		ix_prev = ix;
		iy_prev = iy;
	}

	if (nhits > 1 && i == MIN(ns, SAMPLE)) {
		*ixp = ix;
		*iyp = iy;
		return 1;
	}

	return 0;
}

static int64_t
part2(void)
{
	int vx,vy,vz;
	double ix=0,iy=0,iz=0;

#define RANGE 500
	for (vx=-RANGE; vx<=RANGE; vx++)
	for (vy=-RANGE; vy<=RANGE; vy++)
		if (try_velocity(vx, vy, 0, 1, &ix, &iy))
			goto have_xy;

	return 0;	/* no match */

have_xy:
	for (vz=-RANGE; vz<=RANGE; vz++)
		if (try_velocity(vx, vz, 0, 2, &ix, &iz))
			goto have_z;

	return 0;	/* no match */

have_z:
	return (int64_t)ix + (int64_t)iy + (int64_t)iz;
}

int
main(int argc, char **argv)
{
	int p1, ntok;
	int64_t p2, px,py,pz, vx,vy,vz;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (1) {
		assert(ns < (int)LEN(stones));
		ntok = scanf(
		    " %"SCNd64", %"SCNd64", %"SCNd64" @ "
		    " %"SCNd64", %"SCNd64", %"SCNd64,
		    &px, &py, &pz, &vx, &vy, &vz);
		if (ntok != 6) break;
		stones[ns].pos[0] = (double)px;
		stones[ns].pos[1] = (double)py;
		stones[ns].pos[2] = (double)pz;
		stones[ns].vel[0] = (double)vx;
		stones[ns].vel[1] = (double)vy;
		stones[ns].vel[2] = (double)vz;
		ns++;
	}

	p1 = part1();
	p2 = part2();

	printf("24: %d %"PRId64"\n", p1, p2);
	return 0;
}
