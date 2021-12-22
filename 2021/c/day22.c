#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct cube { int x0,y0,z0, x1,y1,z1; } cube;

static int
cmp_int(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

static int
cube_contains(const cube *out, const cube *in)
{
	return
	     out->x0 <= in->x0 && in->x1 <= out->x1 &&
	     out->y0 <= in->y0 && in->y1 <= out->y1 &&
	     out->z0 <= in->z0 && in->z1 <= out->z1;
}

static void
test_contains(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	assert( cube_contains(&out, &out));
	assert( cube_contains(&out, &ab));
	assert( cube_contains(&out, &a));
	assert( cube_contains(&out, &b));
	assert( cube_contains(&out, &c));

	assert(!cube_contains(&ab, &out));
	assert( cube_contains(&ab, &ab));
	assert(!cube_contains(&ab, &a));
	assert(!cube_contains(&ab, &b));
	assert(!cube_contains(&ab, &c));

	assert(!cube_contains(&a, &out));
	assert( cube_contains(&a, &ab));
	assert( cube_contains(&a, &a));
	assert(!cube_contains(&a, &b));
	assert(!cube_contains(&a, &c));

	assert(!cube_contains(&b, &out));
	assert( cube_contains(&b, &ab));
	assert(!cube_contains(&b, &a));
	assert( cube_contains(&b, &b));
	assert(!cube_contains(&b, &c));

	assert(!cube_contains(&c, &out));
	assert(!cube_contains(&c, &ab));
	assert(!cube_contains(&c, &a));
	assert(!cube_contains(&c, &b));
	assert( cube_contains(&c, &c));
}

static int
cube_intersect(const cube *a, const cube *b, cube *out)
{
	if (!(a->x0 <= b->x0 ? a->x1 >= b->x0 : a->x0 <= b->x1) ||
	    !(a->y0 <= b->y0 ? a->y1 >= b->y0 : a->y0 <= b->y1) ||
	    !(a->z0 <= b->z0 ? a->z1 >= b->z0 : a->z0 <= b->z1))
		return 0;

	if (out) {
		out->x0 = MAX(a->x0, b->x0); out->x1 = MIN(a->x1, b->x1);
		out->y0 = MAX(a->y0, b->y0); out->y1 = MIN(a->y1, b->y1);
		out->z0 = MAX(a->z0, b->z0); out->z1 = MIN(a->z1, b->z1);
	}

	return 1;
}

static void
test_intersect(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	assert( cube_intersect(&out, &out, NULL));
	assert( cube_intersect(&out, &ab,  NULL));
	assert( cube_intersect(&out, &a,   NULL));
	assert( cube_intersect(&out, &b,   NULL));
	assert( cube_intersect(&out, &c,   NULL));

	assert( cube_intersect(&ab, &out, NULL));
	assert( cube_intersect(&ab, &ab,  NULL));
	assert( cube_intersect(&ab, &a,   NULL));
	assert( cube_intersect(&ab, &b,   NULL));
	assert(!cube_intersect(&ab, &c,   NULL));

	assert( cube_intersect(&a, &out, NULL));
	assert( cube_intersect(&a, &ab,  NULL));
	assert( cube_intersect(&a, &a,   NULL));
	assert( cube_intersect(&a, &b,   NULL));
	assert(!cube_intersect(&a, &c,   NULL));

	assert( cube_intersect(&b, &out, NULL));
	assert( cube_intersect(&b, &ab,  NULL));
	assert( cube_intersect(&b, &a,   NULL));
	assert( cube_intersect(&b, &b,   NULL));
	assert(!cube_intersect(&b, &c,   NULL));

	assert( cube_intersect(&c, &out, NULL));
	assert(!cube_intersect(&c, &ab,  NULL));
	assert(!cube_intersect(&c, &a,   NULL));
	assert(!cube_intersect(&c, &b,   NULL));
	assert( cube_intersect(&c, &c,   NULL));
}

static int
cube_subtract(const cube *a, const cube *b, cube out[27])
{
	int n=0, x,y,z;
	int xs[4] = {a->x0, a->x1+1, b->x0, b->x1+1};
	int ys[4] = {a->y0, a->y1+1, b->y0, b->y1+1};
	int zs[4] = {a->z0, a->z1+1, b->z0, b->z1+1};

	if (cube_contains(b, a)) return 0;
	if (!cube_intersect(a, b, NULL)) { *out=*a; return 1; }

	qsort(xs, 4, sizeof(*xs), cmp_int);
	qsort(ys, 4, sizeof(*ys), cmp_int);
	qsort(zs, 4, sizeof(*zs), cmp_int);

	for (x=0; x<3; x++)
	for (y=0; y<3; y++)
	for (z=0; z<3; z++) {
		out[n].x0 = xs[x]; out[n].x1 = xs[x+1]-1;
		out[n].y0 = ys[y]; out[n].y1 = ys[y+1]-1;
		out[n].z0 = zs[z]; out[n].z1 = zs[z+1]-1;

		n += out[n].x0 <= out[n].x1 &&
		     out[n].y0 <= out[n].y1 &&
		     out[n].z0 <= out[n].z1 &&
		     cube_contains(a, out+n) &&
		    !cube_contains(b, out+n);
	}

	return n;
}

static void
test_subtract(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	static cube buf[27];

	assert(cube_subtract(&out, &out, buf) == 0);
	assert(cube_subtract(&out, &ab,  buf) == 26);
	assert(cube_subtract(&out, &a,   buf) == 7);
	assert(cube_subtract(&out, &b,   buf) == 7);
	assert(cube_subtract(&out, &c,   buf) == 7);

	assert(cube_subtract(&ab, &out, buf) == 0);
	assert(cube_subtract(&ab, &ab,  buf) == 0);
	assert(cube_subtract(&ab, &a,   buf) == 0);
	assert(cube_subtract(&ab, &b,   buf) == 0);
	assert(cube_subtract(&ab, &c,   buf) == 1);

	assert(cube_subtract(&a, &out, buf) == 0);
	assert(cube_subtract(&a, &ab,  buf) == 7);
	assert(cube_subtract(&a, &a,   buf) == 0);
	assert(cube_subtract(&a, &b,   buf) == 7);
	assert(cube_subtract(&a, &c,   buf) == 1);

	assert(cube_subtract(&b, &out, buf) == 0);
	assert(cube_subtract(&b, &ab,  buf) == 7);
	assert(cube_subtract(&b, &a,   buf) == 7);
	assert(cube_subtract(&b, &b,   buf) == 0);
	assert(cube_subtract(&b, &c,   buf) == 1);

	assert(cube_subtract(&c, &out, buf) == 0);
	assert(cube_subtract(&c, &ab,  buf) == 1);
	assert(cube_subtract(&c, &a,   buf) == 1);
	assert(cube_subtract(&c, &b,   buf) == 1);
	assert(cube_subtract(&c, &c,   buf) == 0);
}

#define SZ 10240

int
main(int argc, char **argv)
{
	static const cube p1box = {-50,-50,-50, 50,50,50};
	static cube cubes[2][SZ];
	int ncubes[2]={}, cur=0, i;
	uint64_t p1=0,p2=0;
	cube step;
	char instr[4];

	if (argc > 1 && !strcmp(argv[1], "-t")) {
		test_contains();
		test_intersect();
		test_subtract();
		printf("22: tests ok\n");
		return 0;
	}

	while (scanf(" %3s x=%d..%d,y=%d..%d,z=%d..%d", instr,
	    &step.x0, &step.x1, &step.y0, &step.y1, &step.z0, &step.z1)
	    == 7) {
	    	cur = !cur;

		if (instr[1] == 'n') {
			ncubes[cur] = 1;
			cubes[cur][0] = step;
		} else
			ncubes[cur] = 0;

		for (i=0; i < ncubes[!cur]; i++) {
			assert(ncubes[cur] + 3*3*3 <= SZ);
			ncubes[cur] += cube_subtract(
			    &cubes[!cur][i], &step,
			    &cubes[cur][ncubes[cur]]);
		}
	}

	for (i=0; i < ncubes[cur]; i++) {
		p2 += (uint64_t)(cubes[cur][i].x1+1 - cubes[cur][i].x0) *
		      (uint64_t)(cubes[cur][i].y1+1 - cubes[cur][i].y0) *
		      (uint64_t)(cubes[cur][i].z1+1 - cubes[cur][i].z0);

		if (cube_intersect(&cubes[cur][i], &p1box,
		    &cubes[cur][i]))
			p1 += (cubes[cur][i].x1+1 - cubes[cur][i].x0) *
			      (cubes[cur][i].y1+1 - cubes[cur][i].y0) *
			      (cubes[cur][i].z1+1 - cubes[cur][i].z0);
	}

	printf("22: %"PRIu64" %"PRIu64"\n", p1, p2);
	return 0;
}
