#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define SZ 32

typedef struct pt {
	int x, y;
} Pt;

static int sweep(int, int);
static int ray(int, int, int, int, int *, int *);
static int compar(const void *, const void *);
static int fac(int);
static int gcd(int, int);
static int gcdfac(int);

static char mask[SZ][SZ];
static char grid[SZ][SZ];
static Pt astrs[SZ*SZ];
static Pt hits[SZ*SZ];

static int w, h;
static int nastrs;
static Pt origin; /* for compar() */

int
main()
{
	char *p;
	int bestx=0, sx, dx, x;
	int besty=0, sy, dy, y;
	int bestn=0, n;

	while (fgets(grid[h], SZ, stdin)) {
		if (!(p = strchr(grid[h], '\n')))
			errx(1, "width overflow or no \\n at EOF");
		*p = '\0';
		for (w = 0; grid[h][w]; w++)
			if (grid[h][w] == '#') {
				astrs[nastrs].x = w;
				astrs[nastrs].y = h;
				nastrs++;
			}
		if (++h >= SZ)
			errx(1, "height overflow");
	}

	for (dy = 0; dy < h; dy++)
	for (dx = 0; dx < w; dx++) {
		if (dx == 0 && dy == 0)
			continue;
		for (x = dx * 2, y = dy * 2;
	  	     x >= 0 && x < w && y >= 0 && y < h;
		     x += dx, y += dy)
			mask[y][x] = 1;
	}

	for (sy = 0; sy < h; sy++)
	for (sx = 0; sx < w; sx++) {
		if (grid[sy][sx] != '#')
			continue;
		n = 0;
		for (dy = 1-h; dy < h; dy++)
		for (dx = 1-w; dx < w; dx++)
			if ((dx || dy) && !mask[abs(dx)][abs(dy)])
				n += ray(sx, sy, dx, dy, NULL, NULL);
		if (n > bestn) {
			bestn = n;
			bestx = sx;
			besty = sy;
		}
	}

	printf("Part 1: %d (%d,%d)\n", bestn, bestx, besty);

	fprintf(stderr, " hits: %d\n", sweep(sx, sy));

	printf("Part 2: no solution\n");
	return 0;
}

static int
sweep()
{
	qsort(astrs, nastrs, sizeof(astrs[0]), )
}

static int
ray(int sx, int sy, int dx, int dy, int *hitx, int *hity)
{
	int x, y;

	for (x = sx + dx, y = sy + dy;
	     x >= 0 && x < w && y >= 0 && y < h;
	     x += dx, y += dy)
		if (grid[y][x] == '#') {
			if (hitx) *hitx = x;
			if (hity) *hity = y;
			return 1;
		}
	
	return 0;
}

static int
gcd(int a, int b)
{
	int i;

	a = abs(a); b = abs(b);

	for (i = a>b ? a : b; i > 2; i--)
		if (!(a % i) && !(b % i))
			return i;
	
	return 1;
}

/*
 * Compar points, sorting in this order:
 *  - distance from 'origin'
 *  - angle from 'origin'
 */
static int
compar(const void *ap, const void *bp)
{
	Pt a, b;
	int da2, db2;

	a = *(const Pt *)a;
	b = *(const Pt *)b;

	d = 
}