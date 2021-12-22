#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#define MIN(a,b) ((a)<(b):(a):(b))
#define MAX(a,b) ((a)>(b):(a):(b))

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
cube_intersects(const cube *a, const cube *b)
{
	return
	    (a->x0 <= b->x0 ? a->x1 >= b->x0 : a->x0 <= b->x1) &&
	    (a->y0 <= b->y0 ? a->y1 >= b->y0 : a->y0 <= b->y1) &&
	    (a->z0 <= b->z0 ? a->z1 >= b->z0 : a->z0 <= b->z1);
}

static void
test_intersects(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	assert( cube_intersects(&out, &out));
	assert( cube_intersects(&out, &ab));
	assert( cube_intersects(&out, &a));
	assert( cube_intersects(&out, &b));
	assert( cube_intersects(&out, &c));

	assert( cube_intersects(&ab, &out));
	assert( cube_intersects(&ab, &ab));
	assert( cube_intersects(&ab, &a));
	assert( cube_intersects(&ab, &b));
	assert(!cube_intersects(&ab, &c));

	assert( cube_intersects(&a, &out));
	assert( cube_intersects(&a, &ab));
	assert( cube_intersects(&a, &a));
	assert( cube_intersects(&a, &b));
	assert(!cube_intersects(&a, &c));

	assert( cube_intersects(&b, &out));
	assert( cube_intersects(&b, &ab));
	assert( cube_intersects(&b, &a));
	assert( cube_intersects(&b, &b));
	assert(!cube_intersects(&b, &c));

	assert( cube_intersects(&c, &out));
	assert(!cube_intersects(&c, &ab));
	assert(!cube_intersects(&c, &a));
	assert(!cube_intersects(&c, &b));
	assert( cube_intersects(&c, &c));
}

#define OP_ADD 1
#define OP_SUB 2
#define OP_INT 3

static int
cube_combine(const cube *a, const cube *b, int op, cube out[27])
{
	int x,y,z, n=0, in_a,in_b;

	if (op==OP_ADD && cube_contains(a, b)) { *out=*a; return 1; }
	if (op==OP_ADD && cube_contains(b, a)) { *out=*b; return 1; }
	if (op==OP_ADD && !cube_intersects(a, b))
		{ out[0] = *a; out[1] = *b; return 2; }

	if (op==OP_SUB && cube_contains(b, a)) return 0;
	if (op==OP_SUB && !cube_intersects(a, b)) { *out=*a; return 1; }

	if (op==OP_INT && !cube_intersects(a, b)) return 0;
	if (op==OP_INT && cube_contains(a, b)) { *out=*b; return 1; }
	if (op==OP_INT && cube_contains(b, a)) { *out=*a; return 1; }

	int xs[4] = {a->x0, a->x1+1, b->x0, b->x1+1};
	int ys[4] = {a->y0, a->y1+1, b->y0, b->y1+1};
	int zs[4] = {a->z0, a->z1+1, b->z0, b->z1+1};

	qsort(xs, 4, sizeof(*xs), cmp_int);
	qsort(ys, 4, sizeof(*ys), cmp_int);
	qsort(zs, 4, sizeof(*zs), cmp_int);

	//printf(" xs=%2d %2d %2d %2d\n", xs[0], xs[1], xs[2], xs[3]);
	//printf(" ys=%2d %2d %2d %2d\n", ys[0], ys[1], ys[2], ys[3]);
	//printf(" zs=%2d %2d %2d %2d\n", zs[0], zs[1], zs[2], zs[3]);

	for (x=0; x<3; x++)
	for (y=0; y<3; y++)
	for (z=0; z<3; z++) {
		out[n].x0 = xs[x]; out[n].x1 = xs[x+1]-1;
		out[n].y0 = ys[y]; out[n].y1 = ys[y+1]-1;
		out[n].z0 = zs[z]; out[n].z1 = zs[z+1]-1;

		//printf(" %d,%d,%d: %2d,%2d,%2d  %2d,%2d,%2d  (%d)", x,y,z,
		//    out[n].x0, out[n].y0, out[n].z0,
		//    out[n].x1, out[n].y1, out[n].z1,
		//    (out[n].x1+1 - out[n].x0) *
		//    (out[n].y1+1 - out[n].y0) *
		//    (out[n].x1+1 - out[n].x0));

		if (out[n].x0 > out[n].x1) continue;
		if (out[n].y0 > out[n].y1) continue;
		if (out[n].z0 > out[n].z1) continue;

		//if (out[n].x0 > out[n].x1) { printf(" !x\n"); continue; }
		//if (out[n].y0 > out[n].y1) { printf(" !y\n"); continue; }
		//if (out[n].z0 > out[n].z1) { printf(" !z\n"); continue; }

		in_a = cube_contains(a, out+n);
		in_b = cube_contains(b, out+n);

		//printf(" in_a=%d in_b=%d\n", in_a, in_b);

		switch (op) {
		case OP_ADD: n += in_a ||  in_b; break;
		case OP_SUB: n += in_a && !in_b; break;
		case OP_INT: n += in_a && !in_b; break;
		}
	}

	//printf("%d\n", n);
	return n;
}

static void
test_combine(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	static cube buf[27];

	assert(cube_combine(&out, &out, OP_ADD, buf) == 1);
	assert(cube_combine(&out, &ab,  OP_ADD, buf) == 1);
	assert(cube_combine(&out, &a,   OP_ADD, buf) == 1);
	assert(cube_combine(&out, &b,   OP_ADD, buf) == 1);
	assert(cube_combine(&out, &c,   OP_ADD, buf) == 1);

	assert(cube_combine(&ab, &out, OP_ADD, buf) == 1);
	assert(cube_combine(&ab, &ab,  OP_ADD, buf) == 1);
	assert(cube_combine(&ab, &a,   OP_ADD, buf) == 1);
	assert(cube_combine(&ab, &b,   OP_ADD, buf) == 1);
	assert(cube_combine(&ab, &c,   OP_ADD, buf) == 2);

	assert(cube_combine(&a, &out, OP_ADD, buf) == 1);
	assert(cube_combine(&a, &ab,  OP_ADD, buf) == 1);
	assert(cube_combine(&a, &a,   OP_ADD, buf) == 1);
	assert(cube_combine(&a, &b,   OP_ADD, buf) == 15);
	assert(cube_combine(&a, &c,   OP_ADD, buf) == 2);

	assert(cube_combine(&b, &out, OP_ADD, buf) == 1);
	assert(cube_combine(&b, &ab,  OP_ADD, buf) == 1);
	assert(cube_combine(&b, &a,   OP_ADD, buf) == 15);
	assert(cube_combine(&b, &b,   OP_ADD, buf) == 1);
	assert(cube_combine(&b, &c,   OP_ADD, buf) == 2);

	assert(cube_combine(&c, &out, OP_ADD, buf) == 1);
	assert(cube_combine(&c, &ab,  OP_ADD, buf) == 2);
	assert(cube_combine(&c, &a,   OP_ADD, buf) == 2);
	assert(cube_combine(&c, &b,   OP_ADD, buf) == 2);
	assert(cube_combine(&c, &c,   OP_ADD, buf) == 1);

	assert(cube_combine(&out, &out, OP_SUB, buf) == 0);
	assert(cube_combine(&out, &ab,  OP_SUB, buf) == 26);
	assert(cube_combine(&out, &a,   OP_SUB, buf) == 7);
	assert(cube_combine(&out, &b,   OP_SUB, buf) == 7);
	assert(cube_combine(&out, &c,   OP_SUB, buf) == 7);

	assert(cube_combine(&ab, &out, OP_SUB, buf) == 0);
	assert(cube_combine(&ab, &ab,  OP_SUB, buf) == 0);
	assert(cube_combine(&ab, &a,   OP_SUB, buf) == 0);
	assert(cube_combine(&ab, &b,   OP_SUB, buf) == 0);
	assert(cube_combine(&ab, &c,   OP_SUB, buf) == 1);

	assert(cube_combine(&a, &out, OP_SUB, buf) == 0);
	assert(cube_combine(&a, &ab,  OP_SUB, buf) == 7);
	assert(cube_combine(&a, &a,   OP_SUB, buf) == 0);
	assert(cube_combine(&a, &b,   OP_SUB, buf) == 7);
	assert(cube_combine(&a, &c,   OP_SUB, buf) == 1);

	assert(cube_combine(&b, &out, OP_SUB, buf) == 0);
	assert(cube_combine(&b, &ab,  OP_SUB, buf) == 7);
	assert(cube_combine(&b, &a,   OP_SUB, buf) == 7);
	assert(cube_combine(&b, &b,   OP_SUB, buf) == 0);
	assert(cube_combine(&b, &c,   OP_SUB, buf) == 1);

	assert(cube_combine(&c, &out, OP_SUB, buf) == 0);
	assert(cube_combine(&c, &ab,  OP_SUB, buf) == 1);
	assert(cube_combine(&c, &a,   OP_SUB, buf) == 1);
	assert(cube_combine(&c, &b,   OP_SUB, buf) == 1);
	assert(cube_combine(&c, &c,   OP_SUB, buf) == 0);

	assert( cube_combine(&out, &out, OP_INT, buf));
	assert( cube_combine(&out, &ab,  OP_INT, buf));
	assert( cube_combine(&out, &a,   OP_INT, buf));
	assert( cube_combine(&out, &b,   OP_INT, buf));
	assert( cube_combine(&out, &c,   OP_INT, buf));

	assert( cube_combine(&ab, &out, OP_INT, buf));
	assert( cube_combine(&ab, &ab,  OP_INT, buf));
	assert( cube_combine(&ab, &a,   OP_INT, buf));
	assert( cube_combine(&ab, &b,   OP_INT, buf));
	assert(!cube_combine(&ab, &c,   OP_INT, buf));

	assert( cube_combine(&a, &out, OP_INT, buf));
	assert( cube_combine(&a, &ab,  OP_INT, buf));
	assert( cube_combine(&a, &a,   OP_INT, buf));
	assert( cube_combine(&a, &b,   OP_INT, buf));
	assert(!cube_combine(&a, &c,   OP_INT, buf));

	assert( cube_combine(&b, &out, OP_INT, buf));
	assert( cube_combine(&b, &ab,  OP_INT, buf));
	assert( cube_combine(&b, &a,   OP_INT, buf));
	assert( cube_combine(&b, &b,   OP_INT, buf));
	assert(!cube_combine(&b, &c,   OP_INT, buf));

	assert( cube_combine(&c, &out, OP_INT, buf));
	assert(!cube_combine(&c, &ab,  OP_INT, buf));
	assert(!cube_combine(&c, &a,   OP_INT, buf));
	assert(!cube_combine(&c, &b,   OP_INT, buf));
	assert( cube_combine(&c, &c,   OP_INT, buf));
}

#define SZ 10240

int
main(int argc, char **argv)
{
	static const cube p1box = {-50,-50,-50, 50,50,50};
	static cube cubes[2][SZ];
	int ncubes[2]={}, cur=0, i,x,y,z;
	uint64_t p1=0,p2=0, n;
	cube step, *c;
	char instr[4];

	if (argc > 1 && !strcmp(argv[1], "-t")) {
		test_contains();
		test_intersects();
		test_combine();
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
			ncubes[cur] += cube_combine(
			    &cubes[!cur][i], &step, OP_SUB,
			    &cubes[cur][ncubes[cur]]);
		}

		//printf("step\n");
		for (n=0, i=0; i < ncubes[cur]; i++) {
			//printf(" cube %d\n", i);

			n += (cubes[cur][i].x1+1 - cubes[cur][i].x0) *
			     (cubes[cur][i].y1+1 - cubes[cur][i].y0) *
			     (cubes[cur][i].z1+1 - cubes[cur][i].z0);

			//for (x = cubes[cur][i].x0; x <= cubes[cur][i].x1; x++)
			//for (y = cubes[cur][i].y0; y <= cubes[cur][i].y1; y++)
			//for (z = cubes[cur][i].z0; z <= cubes[cur][i].z1; z++) {
				n++;
				//printf("  %2d,%2d,%2d\n", x,y,z);
			//}
		}

		printf("   %d cubes, %"PRIu64" cells\n", ncubes[cur], n);
	}

	for (i=0; i < ncubes[cur]; i++)
		p2 += (uint64_t)(cubes[cur][i].x1+1 - cubes[cur][i].x0) *
		      (uint64_t)(cubes[cur][i].y1+1 - cubes[cur][i].y0) *
		      (uint64_t)(cubes[cur][i].z1+1 - cubes[cur][i].z0);

	cur = !cur;
	ncubes[cur] = 0;

	for (i=0; i < ncubes[!cur]; i++) {
		assert(ncubes[cur] + 3*3*3 <= SZ);
		ncubes[cur] += cube_combine(
		    &cubes[!cur][i], &p1box, OP_INT,
		    &cubes[cur][ncubes[cur]]);
	}

	for (i=0; i < ncubes[cur]; i++)
		p1 += (cubes[cur][i].x1+1 - cubes[cur][i].x0) *
		      (cubes[cur][i].y1+1 - cubes[cur][i].y0) *
		      (cubes[cur][i].z1+1 - cubes[cur][i].z0);

	printf("22: %"PRIu64" %"PRIu64"\n", p1, p2);
	return 0;
}
