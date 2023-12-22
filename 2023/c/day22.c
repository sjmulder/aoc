#include "common.h"

#define XSZ 10
#define YSZ 10
#define ZSZ 400

struct shape { int x1,y1,z1, x2,y2,z2; };

struct brick {
	struct shape shape;
	struct brick *above[5]; int nabove;
	struct brick *below[5]; int nbelow;
};

static struct brick bricks[1600];
static struct brick *grid[ZSZ][YSZ][XSZ];
static int nbricks;

static void
paint(struct shape *r, struct brick *brick)
{
	int x,y,z;

	for (z=r->z1; z<=r->z2; z++)
	for (y=r->y1; y<=r->y2; y++)
	for (x=r->x1; x<=r->x2; x++)
		grid[z][y][x] = brick;
}

static int
scan(struct shape *r, struct brick **hits, int sz)
{
	static struct brick *buf[5];
	struct brick *hit;
	int nhits=0, x,y,z, i;

	if (!hits) {
		hits = buf;
		sz = (int)LEN(buf);
	}

	for (z=r->z1; z<=r->z2; z++)
	for (y=r->y1; y<=r->y2; y++)
	for (x=r->x1; x<=r->x2; x++) {
		if (!(hit = grid[z][y][x]))
			continue;
		for (i=0; i<nhits && hit != hits[i]; i++)
			;
		if (i == nhits) {
			assert(nhits < sz);
			hits[nhits++] = hit;
		}
	}

	return nhits;
}

static void
rescan(struct brick *brick)
{
	struct shape above, below;

	above = brick->shape;
	above.z1 = above.z2 = above.z2+1;
	brick->nabove = scan(&above, brick->above,
	    (int)LEN(brick->above));

	below = brick->shape;
	below.z1 = below.z2 = below.z1-1;
	brick->nbelow = scan(&below, brick->below,
	    (int)LEN(brick->below));
}

static void
gravitas(void)
{
	int dirty, i;

	for (dirty=1; dirty; )
	for (dirty=0, i=0; i<nbricks; i++) {
		rescan(&bricks[i]);

		if (!bricks[i].shape.z1)
			continue;

		if (!bricks[i].nbelow) {
			paint(&bricks[i].shape, NULL);
			bricks[i].shape.z1--;
			bricks[i].shape.z2--;
			paint(&bricks[i].shape, &bricks[i]);
			dirty = 1;
		}
	}
}

static int
jenga(void)
{
	int count=0, i,j;

	for (i=0; i<nbricks; i++) {
		for (j=0; j<bricks[i].nabove; j++)
			if (bricks[i].above[j]->nbelow < 2)
				break;
		count += j == bricks[i].nabove;
	}

	return count;
}

int
main(int argc, char **argv)
{
	struct shape *r;
	int p1, ntok;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (; ; nbricks++) {
		assert(nbricks < (int)LEN(bricks));
		r = &bricks[nbricks].shape;
		ntok = scanf("%d,%d,%d~%d,%d,%d",
		    &r->x1, &r->y1, &r->z1,
		    &r->x2, &r->y2, &r->z2);
		if (ntok < 6)
			break;

		assert(r->x1 <= r->x2);
		assert(r->y1 <= r->y2);
		assert(r->z1 <= r->z2);

		assert(r->x1 >= 0); assert(r->x1 < XSZ);
		assert(r->x2 >= 0); assert(r->x2 < XSZ);
		assert(r->y1 >= 0); assert(r->y1 < YSZ);
		assert(r->y2 >= 0); assert(r->y2 < YSZ);
		assert(r->z1 >= 0); assert(r->z1 < ZSZ);
		assert(r->z2 >= 0); assert(r->z2 < ZSZ);

		paint(r, &bricks[nbricks]);
	}

	gravitas();
	p1 = jenga();

	printf("22: %d <TODO>\n", p1);
	return 0;
}
