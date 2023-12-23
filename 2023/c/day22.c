#include "common.h"

#define XSZ 10
#define YSZ 10
#define ZSZ 400

struct block { int x1,y1,z1, x2,y2,z2; };

static struct block bricks[1600];
static struct block *grid[ZSZ][YSZ][XSZ];
static size_t nbricks;

static struct block
above(struct block b)
{
	b.z1 = b.z2 = b.z2+1;
	return b;
}

static struct block
below(struct block b)
{
	b.z1 = b.z2 = b.z1-1;
	return b;
}

static void
paint(struct block b, struct block *val)
{
	int x,y,z;

	for (z=b.z1; z<=b.z2; z++)
	for (y=b.y1; y<=b.y2; y++)
	for (x=b.x1; x<=b.x2; x++)
		grid[z][y][x] = val;
}

static size_t
scan(struct block b, struct block **hits, size_t sz)
{
	static struct block *buf[5];
	struct block *hit;
	size_t nhits=0, i;
	int x,y,z;

	if (!hits) { hits = buf; sz = LEN(buf); }

	for (z=b.z1; z<=b.z2; z++)
	for (y=b.y1; y<=b.y2; y++)
	for (x=b.x1; x<=b.x2; x++) {
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
	size_t i;
	int dirty;

	for (dirty=1; dirty; )
	for (dirty=0, i=0; i<nbricks; i++)
		if (bricks[i].z1 && !scan(below(bricks[i]), NULL, 0)) {
			paint(below(bricks[i]), &bricks[i]);
			bricks[i].z1--;
			bricks[i].z2--;
			paint(above(bricks[i]), NULL);
			dirty = 1;
		}
}

static int
jenga(void)
{
	static struct block *hits[5];
	size_t nabove, i,j;
	int count=0;

	for (i=0; i<nbricks; i++) {
		nabove = scan(above(bricks[i]), hits, (int)LEN(hits));
		for (j=0; j<nabove; j++)
			if (scan(below(*hits[j]), NULL, 0) < 2)
				break;

		count += j == nabove;
	}

	return count;
}

static void
disintegrate(struct block *b, uint8_t *gone)
{
	struct block *up[5], *down[5];
	size_t nup, ndown, i,j;

	if (gone[b-bricks])
		return;
	
	gone[b-bricks] = 1;

	nup = scan(above(*b), up, LEN(up));
	for (i=0; i<nup; i++) {
		ndown = scan(below(*up[i]), down, LEN(down));
		for (j=0; j<ndown; j++)
			if (!gone[down[j]-bricks])
				break;

		if (j == ndown)
			disintegrate(up[i], gone);
	}
}

static int
godzilla(void)
{
	static uint8_t gone[LEN(bricks)];
	size_t i,j;
	int ngone=0;

	for (i=0; i<nbricks; i++) {
		memset(gone, 0, sizeof(gone));
		disintegrate(&bricks[i], gone);

		for (j=0; j<nbricks; j++)
			ngone += gone[j];
	}

	return ngone - nbricks;	/* don't count fist bricks */
}

int
main(int argc, char **argv)
{
	struct block *b;
	int p1,p2, ntok;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (; ; nbricks++) {
		assert(nbricks < LEN(bricks));
		b = &bricks[nbricks];
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

		paint(*b, b);
	}

	gravitas();
	p1 = jenga();
	p2 = godzilla();

	printf("22: %d %d\n", p1, p2);
	return 0;
}
