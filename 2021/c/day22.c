#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

/*
 * Initially I thought I needed 3 cuboid operation: addition,
 * subtraction and intersection, and came up with a generic cuboid
 * masking function.
 *
 * Essentially, it solves the problem of masking (subtraction/addition)
 * yielding non-cuboid shapes by cutting up the bounding cuboid along
 * each of the two cuboid's edges. E.g. in this 1D example:
 *
 *   ┌──┐
 *   └──┘
 *     ┌──┐
 *     └──┘
 *   | || |  cut along each edge
 *   ┌─┬┬─┐
 *   └─┴┴─┘
 *
 * Each resulting cuboid will either be wholly inside or outside each of
 * the two inputs and can then easily be filtered by the desired masking
 * operation. Downside is that it generates far more cuboids then are
 * needed (up to 3*3*3=27 in 3D), e.g. consider a 2D subtraction that
 * punches a hole:
 *
 *   ┌┬──┬┐
 *   ├┼──┼┤
 *   ││  ││
 *   ├┼──┼┤
 *   └┴──┴┘
 *
 * *However* I later realised that with my solution I didn't need to do
 * addition (unions) and that this is way overkill for intersection. So
 * I rewrote it as a not fancy, not generic handwritten subtract() which
 * only yields up to 6 cuboids in a fixed pattern. A 2D version of that
 * pattern looks like this:
 *
 *   ┌────┐
 *   ├┬──┬┤
 *   ││xx││
 *   ├┴──┴┤
 *   └────┘
 *
 * As for the accumulation and double counting problem: I settled on the
 * following pseudocode:
 *
 *   for every instruction
 *     for every cuboid 'c' in list
 *       subtract the new cuboid from 'c'
 *     if 'add'
 *       append the new cuboid
 *
 * In other terms: punch a 'new cuboid'-shaped hole, then append the new
 * cuboid.
 *
 * To prevent having to shift large arrays to accommodate for split
 * cuboids (or having to deal with linked lists and their perf.) I'm
 * using a 'double buffer' for the set of cuboids.
 */

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct cube { int x0,y0,z0, x1,y1,z1; } cube;

static int
contains(const cube *out, const cube *in)
{
	return
	     out->x0 <= in->x0 && in->x1 <= out->x1 &&
	     out->y0 <= in->y0 && in->y1 <= out->y1 &&
	     out->z0 <= in->z0 && in->z1 <= out->z1;
}

static int
intersect(const cube *a, const cube *b, cube *out)
{
	if ((a->x0 <= b->x0 ? a->x1 < b->x0 : a->x0 > b->x1) ||
	    (a->y0 <= b->y0 ? a->y1 < b->y0 : a->y0 > b->y1) ||
	    (a->z0 <= b->z0 ? a->z1 < b->z0 : a->z0 > b->z1))
		return 0;

	if (out) {
		out->x0 = MAX(a->x0, b->x0); out->x1 = MIN(a->x1, b->x1);
		out->y0 = MAX(a->y0, b->y0); out->y1 = MIN(a->y1, b->y1);
		out->z0 = MAX(a->z0, b->z0); out->z1 = MIN(a->z1, b->z1);
	}

	return 1;
}

static int
subtract(const cube *a, const cube *b, cube out[6])
{
	int n=0;

	if (contains(b, a)) return 0;
	if (!intersect(a, b, NULL)) { *out=*a; return 1; }

	if (a->z0 < b->z0) {
		out[n] = *a;
		out[n].z1 = b->z0 -1;
		n++;
	}

	if (a->z1 > b->z1) {
		out[n] = *a;
		out[n].z0 = b->z1 +1;
		n++;
	}

	if (a->y0 < b->y0) {
		out[n] = *a;
		out[n].z0 = MAX(a->z0, b->z0);
		out[n].z1 = MIN(a->z1, b->z1);
		out[n].y1 = b->y0 -1;
		n++;
	}

	if (a->y1 > b->y1) {
		out[n] = *a;
		out[n].z0 = MAX(a->z0, b->z0);
		out[n].z1 = MIN(a->z1, b->z1);
		out[n].y0 = b->y1 +1;
		n++;
	}

	if (a->x0 < b->x0) {
		out[n] = *a;
		out[n].z0 = MAX(a->z0, b->z0);
		out[n].y0 = MAX(a->y0, b->y0);
		out[n].z1 = MIN(a->z1, b->z1);
		out[n].y1 = MIN(a->y1, b->y1);
		out[n].x1 = b->x0 -1;
		n++;
	}

	if (a->x1 > b->x1) {
		out[n] = *a;
		out[n].z0 = MAX(a->z0, b->z0);
		out[n].y0 = MAX(a->y0, b->y0);
		out[n].z1 = MIN(a->z1, b->z1);
		out[n].y1 = MIN(a->y1, b->y1);
		out[n].x0 = b->x1 +1;
		n++;
	}

	return n;
}

#define SZ 4069

int
main()
{
	static const cube p1box = {-50,-50,-50, 50,50,50};
	static cube cubes[2][SZ];
	int num[2]={}, cur=0, i;
	uint64_t p1=0,p2=0;
	cube step;
	char instr[4];

	while (scanf(" %3s x=%d..%d,y=%d..%d,z=%d..%d", instr,
	    &step.x0, &step.x1, &step.y0, &step.y1, &step.z0, &step.z1)
	    == 7) {
	    	cur = !cur;

		if (instr[1] == 'n') {
			num[cur] = 1;
			cubes[cur][0] = step;
		} else
			num[cur] = 0;

		for (i=0; i < num[!cur]; i++) {
			assert(num[cur] +6 <= SZ);
			num[cur] += subtract(&cubes[!cur][i], &step,
			    &cubes[cur][num[cur]]);
		}
	}

	for (i=0; i < num[cur]; i++) {
		p2 += (uint64_t)(cubes[cur][i].x1+1 - cubes[cur][i].x0) *
		      (uint64_t)(cubes[cur][i].y1+1 - cubes[cur][i].y0) *
		      (uint64_t)(cubes[cur][i].z1+1 - cubes[cur][i].z0);

		if (intersect(&cubes[cur][i], &p1box, &cubes[cur][i]))
			p1 += (cubes[cur][i].x1+1 - cubes[cur][i].x0) *
			      (cubes[cur][i].y1+1 - cubes[cur][i].y0) *
			      (cubes[cur][i].z1+1 - cubes[cur][i].z0);
	}

	printf("22: %"PRIu64" %"PRIu64"\n", p1, p2);
	return 0;
}
