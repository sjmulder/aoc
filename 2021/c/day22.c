#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

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

static void
test_contains(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	assert( contains(&out, &out));
	assert( contains(&out, &ab));
	assert( contains(&out, &a));
	assert( contains(&out, &b));
	assert( contains(&out, &c));

	assert(!contains(&ab, &out));
	assert( contains(&ab, &ab));
	assert(!contains(&ab, &a));
	assert(!contains(&ab, &b));
	assert(!contains(&ab, &c));

	assert(!contains(&a, &out));
	assert( contains(&a, &ab));
	assert( contains(&a, &a));
	assert(!contains(&a, &b));
	assert(!contains(&a, &c));

	assert(!contains(&b, &out));
	assert( contains(&b, &ab));
	assert(!contains(&b, &a));
	assert( contains(&b, &b));
	assert(!contains(&b, &c));

	assert(!contains(&c, &out));
	assert(!contains(&c, &ab));
	assert(!contains(&c, &a));
	assert(!contains(&c, &b));
	assert( contains(&c, &c));
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

static void
test_intersect(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	assert( intersect(&out, &out, NULL));
	assert( intersect(&out, &ab,  NULL));
	assert( intersect(&out, &a,   NULL));
	assert( intersect(&out, &b,   NULL));
	assert( intersect(&out, &c,   NULL));

	assert( intersect(&ab, &out, NULL));
	assert( intersect(&ab, &ab,  NULL));
	assert( intersect(&ab, &a,   NULL));
	assert( intersect(&ab, &b,   NULL));
	assert(!intersect(&ab, &c,   NULL));

	assert( intersect(&a, &out, NULL));
	assert( intersect(&a, &ab,  NULL));
	assert( intersect(&a, &a,   NULL));
	assert( intersect(&a, &b,   NULL));
	assert(!intersect(&a, &c,   NULL));

	assert( intersect(&b, &out, NULL));
	assert( intersect(&b, &ab,  NULL));
	assert( intersect(&b, &a,   NULL));
	assert( intersect(&b, &b,   NULL));
	assert(!intersect(&b, &c,   NULL));

	assert( intersect(&c, &out, NULL));
	assert(!intersect(&c, &ab,  NULL));
	assert(!intersect(&c, &a,   NULL));
	assert(!intersect(&c, &b,   NULL));
	assert( intersect(&c, &c,   NULL));
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

static void
test_subtract(void)
{
	static const cube out = {0,0,0, 2,2,2};
	static const cube ab  = {1,1,1, 1,1,1}; /* a,b intersection */
	static const cube a   = {0,0,0, 1,1,1};
	static const cube b   = {1,1,1, 2,2,2};
	static const cube c   = {0,0,2, 0,0,2};

	static cube buf[27];

	assert(subtract(&out, &out, buf) == 0);
	assert(subtract(&out, &ab,  buf) == 6);
	assert(subtract(&out, &a,   buf) == 3);
	assert(subtract(&out, &b,   buf) == 3);
	assert(subtract(&out, &c,   buf) == 3);

	assert(subtract(&ab, &out, buf) == 0);
	assert(subtract(&ab, &ab,  buf) == 0);
	assert(subtract(&ab, &a,   buf) == 0);
	assert(subtract(&ab, &b,   buf) == 0);
	assert(subtract(&ab, &c,   buf) == 1);

	assert(subtract(&a, &out, buf) == 0);
	assert(subtract(&a, &ab,  buf) == 3);
	assert(subtract(&a, &a,   buf) == 0);
	assert(subtract(&a, &b,   buf) == 3);
	assert(subtract(&a, &c,   buf) == 1);

	assert(subtract(&b, &out, buf) == 0);
	assert(subtract(&b, &ab,  buf) == 3);
	assert(subtract(&b, &a,   buf) == 3);
	assert(subtract(&b, &b,   buf) == 0);
	assert(subtract(&b, &c,   buf) == 1);

	assert(subtract(&c, &out, buf) == 0);
	assert(subtract(&c, &ab,  buf) == 1);
	assert(subtract(&c, &a,   buf) == 1);
	assert(subtract(&c, &b,   buf) == 1);
	assert(subtract(&c, &c,   buf) == 0);
}

#define SZ 4069

int
main(int argc, char **argv)
{
	static const cube p1box = {-50,-50,-50, 50,50,50};
	static cube cubes[2][SZ];
	int num[2]={}, cur=0, i;
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
