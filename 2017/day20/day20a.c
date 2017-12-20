#define _CRT_SECURE_NO_WARNINGS
#define LEN(a) (sizeof(a)/sizeof(*a))

#include <stdio.h>
#include <stdlib.h>

struct vec { int x, y, z; };
struct part { struct vec pos, vel, acc; };

static int
sumvec(struct vec v)
{
	return abs(v.x) + abs(v.y) + abs(v.z);
}

static void
printvec(struct vec v)
{
	printf("<%5d,%5d,%5d> (%5d)", v.x, v.y, v.z, sumvec(v));
}

static void
printpart(struct part *p)
{
	fputs("p=", stdout);
	printvec(p->pos);
	fputs(", v=", stdout);
	printvec(p->pos);
	fputs(", a=", stdout);
	printvec(p->acc);
	putchar('\n');
}

static int
readpart(struct part *p)
{
	return scanf(" p=< %d, %d, %d>, v=< %d, %d, %d>, a=< %d, %d, %d>",
			&p->pos.x, &p->pos.y, &p->pos.z,
			&p->vel.x, &p->vel.y, &p->vel.z,
			&p->acc.x, &p->acc.y, &p->acc.z) == 9;
}

int
main()
{
	struct part ps[1024];
	size_t n=0, i, min=0;
	int d;

	while (readpart(&ps[n])) {
		if (++n >= LEN(ps)) {
			fputs("input too large\n", stderr);
			return 1;
		}
	}

	if (!n) {
		fputs("empty input\n", stderr);
		return 1;
	}

	fputs("   0: ", stdout);
	printpart(ps);
	for (i=1; i<n; i++) {
		if (!(d = sumvec(ps[min].acc) - sumvec(ps[i].acc)))
			if (!(d = sumvec(ps[min].vel) - sumvec(ps[i].vel)))
				d = sumvec(ps[min].pos) - sumvec(ps[i].pos);
		if (d>0) {
			min = i;
			printf("%4zu: ", min);
			printpart(&ps[min]);
		}
	}

	printf("%zu\n", min);
	return 0;
}
