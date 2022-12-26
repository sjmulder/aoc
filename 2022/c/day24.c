#include "common.h"

#define GSZ	128
#define TEND	4096

struct bl { int x,y, dx,dy; };

static uint8_t reach[GSZ][GSZ][2];	/* reachable at t%2? */
static uint8_t blocked[GSZ][GSZ];	/* blocked at t? */
static struct bl bls[GSZ*GSZ];		/* blizzards */
static int gw,gh, nbl;			/* size, blizzard count */

static void vis24_begin(void);
static void vis24_emit(int t);
static void vis24_end(void);

static void
add_bl(int x, int y, int dx, int dy)
{
	bls[nbl].x = x;
	bls[nbl].y = y;
	bls[nbl].dx = dx;
	bls[nbl].dy = dy;
	nbl++;
}

static int
route(int t, int sx, int sy, int ex, int ey)
{
	int x,y, c,p, i;	/* c,p: cur, prev t%2 */

	memset(reach, 0, sizeof(reach));
	reach[sy][sx][t%2] = 1;

	vis24_emit(t);

	for (t=t+1; t<TEND; t++) {
		c = t%2; p = !c;

		for (i=0; i<nbl; i++)
			blocked[bls[i].y][bls[i].x] = 0;

		for (i=0; i<nbl; i++) {
			bls[i].x += bls[i].dx;
			bls[i].y += bls[i].dy;

			if (!bls[i].x) bls[i].x = gw-2;
			if (!bls[i].y) bls[i].y = gh-2;

			if (bls[i].x==gw-1) bls[i].x = 1;
			if (bls[i].y==gh-1) bls[i].y = 1;

			blocked[bls[i].y][bls[i].x] = 1;
		}

		for (y=0; y<gh; y++)
		for (x=0; x<gw; x++)
			reach[y][x][c] = !blocked[y][x] && (
			    reach[y][x][p] ||
			    (y>0    && reach[y-1][x][p]) ||
			    (y<gh-1 && reach[y+1][x][p]) ||
			    (x>0    && reach[y][x-1][p]) ||
			    (x<gw-1 && reach[y][x+1][p]));

		vis24_emit(t);

		if (reach[ey][ex][c])
			break;
	}

	assert(t != TEND);

	return t;
}

int
main(int argc, char **argv)
{
	char buf[GSZ], *lf;
	int x,t, p1,p2;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	while (fgets(buf, sizeof(buf), stdin)) {
		assert(gh < GSZ);

		if (!gw) {
			lf = strchr(buf, '\n');
			gw = lf ? (int)(lf-buf) : (int)strlen(buf);
		}

		for (x=0; buf[x]; x++)
			switch (buf[x]) {
			case '#': blocked[gh][x] = 1; break;
			case '^': add_bl(x, gh, 0,-1); break;
			case 'v': add_bl(x, gh, 0, 1); break;
			case '<': add_bl(x, gh,-1, 0); break;
			case '>': add_bl(x, gh, 1, 0); break;
			}
		gh++;
	}

	vis24_begin();
	vis24_emit(0);

	t = route(0, 1,0, gw-2,gh-1); p1 = t;
	t = route(t, gw-2,gh-1, 1,0);
	t = route(t, 1,0, gw-2,gh-1); p2 = t;

	vis24_end();

	printf("24: %d %d\n", p1,p2);
	return 0;
}

#ifdef WITH_VIS
#include <unistd.h>
#include "vis.h"

#define SCALE	8
#define SLOW	1

struct vis vis;

static void
vis24_begin(void)
{
	vis_begin(&vis, "day24.mp4", 30,
	    (size_t)gw*SCALE,
	    (size_t)gh*SCALE);
}


static void
vis24_emit(int t)
{
	int x,y;
	uint8_t r,g,b;
	float f;

	for (y=0; y<gh; y++)
	for (x=0; x<gw; x++) {
		if (blocked[y][x])
			r=g=b= 32;//255;
		else if (!reach[y][x][t%2])
			r=g=b= 0;
		else {
			f = (float)(t % 50) * 0.02f;
			lerp_rainbow(f, &r,&g,&b);
		}

		vis_fill(&vis,
		    (size_t)x *SCALE,
		    (size_t)y *SCALE,
		    SCALE, SCALE,
		    r,g,b);
	}

	vis_emit(&vis, SLOW);
}

static void
vis24_end(void)
{
	vis_end(&vis);
}
#else
static void vis24_begin(void) {}
static void vis24_emit(int t) { (void)t; }
static void vis24_end(void) {}
#endif
