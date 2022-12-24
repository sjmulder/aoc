#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#define GSZ	128
#define TEND	128
#define INF	(INT_MAX/2)

struct bl { int x0,y0, dx,dy; };

static int dist[GSZ][GSZ][TEND];	/* 'best distance' map */
static uint8_t blocked[GSZ][GSZ][TEND];
static struct bl bls[GSZ*GSZ];		/* blizzards */
static int gw,gh,/*sx,sy=1,*/ex,ey, nbl;/* size, start, end, bl.count */

static int min(int a, int b) { return a<b ? a : b; }

static void vis24_begin(void);
static void vis24_emit(int t);
static void vis24_end(void);

static void
add_bl(int x0, int y0, int dx, int dy)
{
	bls[nbl].x0 = x0;
	bls[nbl].y0 = y0;
	bls[nbl].dx = dx;
	bls[nbl].dy = dy;
	nbl++;
}

static void
add_wall(int x, int y)
{
	int t;

	for (t=0; t<TEND; t++)
		blocked[y][x][t] = 1;
}

static void
trace_bl(struct bl *bl)
{

	int x,y,t;

	x = bl->x0;
	y = bl->y0;

	for (t=0; t<TEND; t++) {
		blocked[y][x][t] = 1;

		x += bl->dx;
		y += bl->dy;

		if (!x) x = gw-2; else if (x==gw-1) x = 1;
		if (!y) y = gh-2; else if (y==gh-1) y = 1;
	}
}

int
main()
{
	char buf[GSZ], *lf;
	int x,y,t, i, d;

	while (fgets(buf, sizeof(buf), stdin)) {
		assert(gh < GSZ);

		if (!gw) {
			lf = strchr(buf, '\n');
			gw = lf ? lf-buf : (int)strlen(buf);
		}

		for (x=0; buf[x]; x++)
			switch (buf[x]) {
			case '#': add_wall(x, gh); break;
			case '^': add_bl(x, gh, 0,-1); break;
			case 'v': add_bl(x, gh, 0, 1); break;
			case '<': add_bl(x, gh,-1, 0); break;
			case '>': add_bl(x, gh, 1, 0); break;
			}
		gh++;
	}

	for (y=0; y<gh; y++)
	for (x=0; x<gh; x++)
		dist[y][x][0] = INF;

	for (i=0; i<nbl; i++)
		trace_bl(&bls[i]);

	ex = gw-2;
	ey = gh-1;

	dist[ey][ex][0] = 0;

	vis24_begin();
	vis24_emit(0);

	for (t=1; t<TEND; t++) {
		for (y=0; y<gh; y++)
		for (x=0; x<gh; x++) {
			if (blocked[y][x][t]) {
				dist[y][x][t] = INF;
				continue;
			}

			d = dist[y][x][t-1];

			if (y>0)    d = min(d, dist[y-1][x][t-1]+1);
			if (y<gh-1) d = min(d, dist[y+1][x][t-1]+1);
			if (x>0)    d = min(d, dist[y][x-1][t-1]+1);
			if (x<gw-1) d = min(d, dist[y][x+1][t-1]+1);

			dist[y][x][t] = d;
		}

		vis24_emit(t);
	}

	vis24_end();

	return 0;
}

#ifdef WITH_VIS
#include <unistd.h>
#include "vis.h"

#define SCALE	20
#define SLOW	30

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
		/*if (blocked[y][x][t])
			r=g=b= 255;
		else*/ if (dist[y][x][t] == INF)
			r=g=b= 0;
		else {
			f = (float)dist[y][x][t] * 0.1f;
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
