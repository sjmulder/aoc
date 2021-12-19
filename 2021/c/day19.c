#include <stdio.h>
#include <err.h>

#define LEN(a)		((int)(sizeof(a)/sizeof(*(a))))
#define MIN(a,b)	((a)<(b)?(a):(b))

typedef struct vec { int x,y,z; } vec;
typedef struct matrix { vec a,b,c; } matrix;

static int cmp_v(vec a, vec b);
static vec add_vv(vec a, vec b);
static vec sub_vv(vec a, vec b);
static vec mul_vm(vec v, const matrix *m);

static const matrix perms[24] = {
	/* north */
	{{ 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1}},
	{{ 0, 0, 1}, { 0, 1, 0}, {-1, 0, 0}},
	{{-1, 0, 0}, { 0,-1, 0}, { 0, 0,-1}},
	{{ 0, 0,-1}, { 0,-1, 0}, { 1, 0, 0}},
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

typedef struct scanner {
	vec bs[32];
	int nb;
} scanner;

static scanner ss[32];
static int ns;

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
		if (s >= LEN(ss)) errx(1, "scanner overflow");
		if (ss[s].nb >= LEN(ss->bs)) errx(1, "beacon overflow");
	}
}

static void
dump(void)
{
	int s,b;

	for (s=0; s<ns; s++) {
		printf("--- scanner %d ---\n", s);
		for (b=0; b<ss[s].nb; b++)
			printf("%d,%d,%d\n",
			    ss[s].bs[b].x,
			    ss[s].bs[b].y,
			    ss[s].bs[b].z);
		putchar('\n');
	}
}

static int
align_scanners(void)
{
	const matrix *perm;
	scanner *s0, *s;
	vec *al0, *al1;	/* beacons in scann.0, scann.s assumed same */
	vec *b0, *b1;	/* beacons in scann.0, scann.s to compare */
	vec off, *b;
	int nmatch;

	s0 = &ss[0];

	for (s=ss+1; s < ss+ns; s++) {
		for (perm = perms; perm < perms+LEN(perms); perm++)
		for (al0 = s0->bs; al0 < s0->bs + s0->nb; al0++)
		for (al1 = s->bs; al1 < s->bs + s->nb; al1++) {
			off = sub_vv(*al0, mul_vm(*al1, perm));
			nmatch = 0;

			for (b0 = s0->bs+1; b0 < s0->bs + s0->nb; b0++)
			for (b1 = s->bs+1;  b1 < s->bs  + s->nb;  b1++)
				nmatch += 0 == cmp_v(*b0,
				    add_vv(off, mul_vm(*b1, perm)));

			printf("s=%ld perm=%ld al0=%ld al1=%ld "
			    "nmatch=%d\n", s-ss, perm-perms, al0-s0->bs,
			    al1-s->bs, nmatch);
			
			if (nmatch >= 12)
				goto match;
			//if (nmatch < MIN(s0->nb, s->nb) - 12)
			//	goto nomatch;
		}

		warnx(1, "no match for scanner %ld\n", ss-s);
		continue;

	match:
		/* transform scanner into scanner 0's coords */
		for (b = s->bs+1; b < s->bs + s->nb; b++)
			*b = add_vv(off, mul_vm(*b, perm));
	}

	return 0;
}

int
main()
{
	read_input();
	//dump();
	align_scanners();
	dump();

	return 0;
}

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
