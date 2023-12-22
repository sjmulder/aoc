#include "common.h"

#define XSZ 10
#define YSZ 10
#define ZSZ 400

struct block { int x1,y1,z1, x2,y2,z2; };

static struct block blocks[1600];
static struct block *grid[ZSZ][YSZ][XSZ];
static int nblocks;

static void
paint(struct block *b, struct block *val)
{
	int x,y,z;

	for (z=b->z1; z<=b->z2; z++)
	for (y=b->y1; y<=b->y2; y++)
	for (x=b->x1; x<=b->x2; x++)
		grid[z][y][x] = val;
}

static int
scan(struct block *b, struct block **hits, int sz)
{
	static struct block *buf[5];
	struct block *hit;
	int nhits=0, x,y,z, i;

	if (!hits) {
		hits = buf;
		sz = (int)LEN(buf);
	}

	for (z=b->z1; z<=b->z2; z++)
	for (y=b->y1; y<=b->y2; y++)
	for (x=b->x1; x<=b->x2; x++) {
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
gravitas(void)
{
	struct block below;
	int dirty, i;

	for (dirty=1; dirty; )
	for (dirty=0, i=0; i<nblocks; i++) {
		if (!blocks[i].z1)
			continue;

		below = blocks[i];
		below.z1 = below.z2 = below.z1-1;

		if (!scan(&below, NULL, 0)) {
			paint(&blocks[i], NULL);
			blocks[i].z1--;
			blocks[i].z2--;
			paint(&blocks[i], &blocks[i]);
			dirty = 1;
		}
	}
}

static int
jenga(void)
{
	static struct block *hits[5];
	struct block above, below;
	int count=0, deps, nabove, i,j;

	for (i=0; i<nblocks; i++) {
		above = blocks[i];
		above.z1 = above.z2 = above.z2+1;
		nabove = scan(&above, hits, (int)LEN(hits));
		deps = 0;

		for (j=0; j<nabove; j++) {
			below = *hits[j];
			below.z1 = below.z2 = below.z1-1;
			deps += scan(&below, NULL, 0) < 2;
		}

		if (!deps)
			count++;
	}

	return count;
}

int
main(int argc, char **argv)
{
	struct block *b;
	int p1, ntok;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (; ; nblocks++) {
		assert(nblocks < (int)LEN(blocks));
		b = &blocks[nblocks];
		ntok = scanf("%d,%d,%d~%d,%d,%d",
		    &b->x1, &b->y1, &b->z1,
		    &b->x2, &b->y2, &b->z2);
		if (ntok < 6)
			break;

		assert(b->x1 <= b->x2);
		assert(b->y1 <= b->y2);
		assert(b->z1 <= b->z2);

		assert(b->x1 >= 0); assert(b->x1 < XSZ);
		assert(b->x2 >= 0); assert(b->x2 < XSZ);
		assert(b->y1 >= 0); assert(b->y1 < YSZ);
		assert(b->y2 >= 0); assert(b->y2 < YSZ);
		assert(b->z1 >= 0); assert(b->z1 < ZSZ);
		assert(b->z2 >= 0); assert(b->z2 < ZSZ);

		paint(b, b);
	}

	gravitas();
	p1 = jenga();

	printf("22: %d <TODO>\n", p1);
	return 0;
}
