#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LEN(a)		((int)(sizeof(a)/sizeof(*(a))))
#define MIN(a,b)	((a)<(b)?(a):(b))

typedef struct vec { int x,y,z; } vec;
typedef struct matrix { vec a,b,c; } matrix;

static const matrix perms[24] = {
	/* north */
	{{ 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1}},
	{{ 0, 0, 1}, { 0, 1, 0}, {-1, 0, 0}},
	{{-1, 0, 0}, { 0, 1, 0}, { 0, 0,-1}},
	{{ 0, 0,-1}, { 0, 1, 0}, { 1, 0, 0}},
	/* east */
	{{ 0, 1, 0}, {-1, 0, 0}, { 0, 0, 1}},
	{{ 0, 1, 0}, { 0, 0,-1}, {-1, 0, 0}},
	{{ 0, 1, 0}, { 1, 0, 0}, { 0, 0,-1}},
	{{ 0, 1, 0}, { 0, 0, 1}, { 1, 0, 0}},
	/* south */
	{{-1, 0, 0}, { 0,-1, 0}, { 0, 0, 1}},
	{{ 0, 0,-1}, { 0,-1, 0}, {-1, 0, 0}},
	{{ 1, 0, 0}, { 0,-1, 0}, { 0, 0,-1}},
	{{ 0, 0, 1}, { 0,-1, 0}, { 1, 0, 0}},
	/* west */
	{{ 0,-1, 0}, { 1, 0, 0}, { 0, 0, 1}},
	{{ 0,-1, 0}, { 0, 0, 1}, {-1, 0, 0}},
	{{ 0,-1, 0}, {-1, 0, 0}, { 0, 0,-1}},
	{{ 0,-1, 0}, { 0, 0,-1}, { 1, 0, 0}},
	/* up */
	{{ 1, 0, 0}, { 0, 0,-1}, { 0, 1, 0}},
	{{ 0, 0,-1}, {-1, 0, 0}, { 0, 1, 0}},
	{{-1, 0, 0}, { 0, 0, 1}, { 0, 1, 0}},
	{{ 0, 0, 1}, { 1, 0, 0}, { 0, 1, 0}},
	/* down */
	{{ 1, 0, 0}, { 0, 0, 1}, { 0,-1, 0}},
	{{ 0, 0, 1}, {-1, 0, 0}, { 0,-1, 0}},
	{{-1, 0, 0}, { 0, 0,-1}, { 0,-1, 0}},
	{{ 0, 0, 1}, { 1, 0, 0}, { 0,-1, 0}}
};

typedef struct scanner scanner;

struct scanner {
	vec bs[32];		/* beacons */
	int nb;

	scanner *base;		/* aligned to this scanner */
	const matrix *perm;	/* ..with rotation..*/
	vec pos;		/* ..and pos (in that frame) */

	vec abs_pos;		/* filled by max_pos() */
};

static scanner ss[33];
static vec bs[33*32];		/* beacons - filled by collect() */
static int ns, nb;

static int
cmp_v(vec a, vec b)
{
	return
	    a.x < b.x ? -1 : a.x > b.x ? 1 :
	    a.y < b.y ? -1 : a.y > b.y ? 1 : a.z - b.z;
}

static vec
add_vv(vec a, vec b)
{
	vec ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}

static vec
sub_vv(vec a, vec b)
{
	vec ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

static vec
mul_vm(vec v, const matrix *m)
{
	vec ret;
	ret.x = v.x * m->a.x + v.y * m->a.y + v.z * m->a.z;
	ret.y = v.x * m->b.x + v.y * m->b.y + v.z * m->b.z;
	ret.z = v.x * m->c.x + v.y * m->c.y + v.z * m->c.z;
	return ret;
}

static void
read_input(void)
{
	static char buf[50];
	int s=0;
	
	while(fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "--- scanner %d", &s) == 1)
			;
		else if (sscanf(buf, "%d,%d,%d",
		    &ss[s].bs[ss[s].nb].x,
		    &ss[s].bs[ss[s].nb].y,
		    &ss[s].bs[ss[s].nb].z) == 3)
		    	ss[s].nb++;

		if (s >= ns) ns = s+1;

		assert(s < LEN(ss));
		assert(ss[s].nb < LEN(ss->bs));
	}
}

static int
test_alignment(scanner *s, scanner *base, const matrix *perm, vec pos)
{
	int nmatch=0;
	int b0i, b1i;
	vec b0, b1;

	for (b0i=0; b0i < base->nb; b0i++) {
		for (b1i=0; b1i < s->nb; b1i++) {
			b0 = base->bs[b0i];

			b1 = s->bs[b1i];
			b1 = mul_vm(b1, perm);
			b1 = add_vv(b1, pos);

			if (cmp_v(b0, b1) == 0) {
				nmatch++;
				goto next;
			}
		}
	next: ;
	}

	return nmatch >= 12;
}

static int
try_align(scanner *s, scanner *base)
{
	int pi, b0i,b1i;
	const matrix *perm;
	vec pos;

	for (pi=0; pi < LEN(perms); pi++)
	for (b0i=0; b0i < base->nb; b0i++)
	for (b1i=0; b1i < s->nb; b1i++) {
		perm = &perms[pi];

		pos = s->bs[b1i];
		pos = mul_vm(pos, perm);
		pos = sub_vv(base->bs[b0i], pos);

		if (test_alignment(s, base, perm, pos)) {
			//printf("match %ld <- %ld perm=%ld "
			//    "pos=%d,%d,%d\n",
			//    base-ss, s-ss, perm-perms,
			//    pos.x,pos.y,pos.z);
			s->base = base;
			s->perm = perm;
			s->pos = pos;
			return 1;
		}
	}

	return 0;
}

static void
align_all(void)
{
	int dirty=1, i,j;

	while (dirty)
		for (dirty=i=0; i<ns; i++) {
			if (i && !ss[i].base)
				continue;
			for (j=1; j<ns; j++) {
				if (ss[j].base)
					continue;
				if (try_align(ss+j, ss+i))
					dirty = 1;
			}
		}
}

static void
collect(void)
{
	int si, bi, i;
	scanner *s;
	vec pos;

	for (si=0; si<ns; si++)
	for (bi=0; bi<ss[si].nb; bi++) {
		s = ss+si;
		pos = s->bs[bi];

		while (s->base) {
			pos = mul_vm(pos, s->perm);
			pos = add_vv(pos, s->pos);

			s = s->base;
		}

		for (i=0; i<nb; i++)
			if (cmp_v(pos, bs[i]) == 0)
				break;

		if (i==nb)
			bs[nb++] = pos;
	}
}

static int
max_dist(void)
{
	scanner *s0, *s;
	int i,j, dist, best=0;

	for (i=0; i<ns; i++) {
		s0 = ss+i;
		s0->abs_pos = s0->pos;

		for (s = s0->base; s && s->base; s = s->base) {
			s0->abs_pos = mul_vm(s0->abs_pos, s->perm);
			s0->abs_pos = add_vv(s0->abs_pos, s->pos);
		}
	}

	for (i=0; i<ns-1; i++)
	for (j=i+1; j<ns; j++) {
		dist = abs(ss[i].abs_pos.x - ss[j].abs_pos.x) +
		       abs(ss[i].abs_pos.y - ss[j].abs_pos.y) +
		       abs(ss[i].abs_pos.z - ss[j].abs_pos.z);
		if (dist > best)
			best = dist;
	}

	return best;
}

int
main()
{
	read_input();
	align_all();
	collect();

	printf("19: %d %d\n", nb, max_dist());
	return 0;
}
