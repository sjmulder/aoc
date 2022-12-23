#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include "compat/util.h"

#ifdef WITH_VIS
# include "vis.h"
#endif

#define GZ	256

enum { RIGHT, DOWN, LEFT, UP };
static const int D[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
static char G[GZ][GZ];
static int gw,gh, cz, px,py;

static void vis22_begin(void);
static void vis22_emit(void);
static void vis22_end(void);

static void
move(int d, int v)
{
	int x0,y0,d0;

	for (; v!=0; v--) {
		x0=px; y0=py; d0=d;

		px += D[d][0];
		py += D[d][1];

#if 0
		if (d == RIGHT && (px>=gw || G[py][px] == ' '))
			for (px=0; G[py][px] == ' '; px++) ;
		else if (d == DOWN && (py>=gh || G[py][px] == ' '))
			for (py=0; G[py][px] == ' '; py++) ;
		else if (d == LEFT && (px<0 || G[py][px] == ' '))
			for (px=gw-1; G[py][px] == ' '; px--) ;
		else if (d == UP && (py<0 || G[py][px] == ' '))
			for (py=gh-1; G[py][px] == ' '; py--) ;
#else
		if (d == RIGHT && (px>=gw || G[py][px] == ' '))
			switch (py/cz) {
			case 0:
				py = (3*cz) - (py%cz) -1;
				px = (2*cz) -1;
				d = LEFT;
				break;
			case 1:
				px = (2*cz) + (py%cz);
				py = (1*cz) -1;
				d = UP;
				break;
			case 2:
				py = (1*cz) - (py%cz) -1;
				px = (3*cz) -1;
				d = LEFT;
				break;
			case 3:
				px = (1*cz) + (py%cz);
				py = (3*cz) -1;
				d = UP;
				break;
			}
		else if (d == DOWN && (py>=gh || G[py][px] == ' '))
			switch (px/cz) {
			case 0:
				px = (2*cz) + (px%cz);
				py = 0;
				d = DOWN;
				break;
			case 1:
				py = (3*cz) + (px%cz);
				px = (1*cz) -1;
				d = LEFT;
				break;
			case 2:
				py = (1*cz) + (px%cz);
				px = (2*cz) -1;
				d = LEFT;
				break;
			}
		else if (d == LEFT && (px<0 || G[py][px] == ' '))
			switch (py/cz) {
			case 0:
				py = (3*cz) - (py%cz) -1;
				px = 0;
				d = RIGHT;
				break;
			case 1:
				px = (0*cz) + (py%cz);
				py = (2*cz);
				d = DOWN;
				break;
			case 2:
				py = (1*cz) - (py%cz) -1;
				px = (1*cz);
				d = RIGHT;
				break;
			case 3:
				px = (1*cz) + (py%cz);
				py = 0;
				d = DOWN;
				break;
			}
		else if (d == UP && (py<0 || G[py][px] == ' '))
			switch (px/cz) {
			case 0:
				py = (1*cz) + (px%cz);
				px = (1*cz);
				d = RIGHT;
				break;
			case 1:
				py = (3*cz) + (px%cz);
				px = 0;
				d = RIGHT;
				break;
			case 2:
				px = (0*cz) + (px%cz);
				py = (4*cz) -1;
				d = UP;
				break;
			}
#endif
		if (G[py][px] == '#') {
			px=x0; py=y0; d=d0;
			return;
		}

		assert(px >= 0); assert(px < gw);
		assert(py >= 0); assert(py < gh);
		assert(G[py][px] == '.');

		vis22_emit();
	}
}

int
main()
{
	static char buf[6144];
	char *lf,*p;
	int d=RIGHT, x,y;

	while (fgets(G[gh], GZ, stdin)) {
		if ((lf = strchr(G[gh], '\n'))) *lf = '\0';
		if (!strlen(G[gh])) break;
		if (!gw) gw = strlen(G[gh]);
		gh++;
		assert(gh < GZ);
	}

	assert(gw/3 == gh/4);
	cz = gw/3;

	p = fgets(buf, sizeof(buf), stdin);
	assert(buf[0]);

	for (y=0; y<gh; y++)
	for (x=0; x<gh; x++)
		if (!G[y][x]) G[y][x] = ' ';

	while (G[0][px] != '.') px++;

	vis22_begin();

	while (*p) {
		     if (*p == 'L') { d = (d+4-1) %4; p++; }
		else if (*p == 'R') { d = (d+1)   %4; p++; }
		else if (isdigit(*p)) move(d, (int)strtol(p, &p, 10));
		else break;
	}

	vis22_end();

	printf("22: %d\n", (py+1)*1000 + (px+1)*4 + d);
	return 0;
}

#ifdef WITH_VIS
# define SCALE	5

static char P[GZ][GZ];	/* path */
static struct vis vis;

static struct vis_grid G_vis = {
	.data = (uint8_t *)G,
	.colors = {
		[' '] = {0, 0, 0},
		['.'] = {128, 255, 128},
		['#'] = {255, 255, 255}
	},
	.w = GZ,
	.h = GZ,
	.cell_sz = SCALE
};

struct vis_grid P_vis = {
	.data = (uint8_t *)P,
	.colors = {
		['@']= {255,255,  0},
		[10] = {250,  0,  0},
		[ 9] = {225, 25, 12},
		[ 8] = {200, 50, 25},
		[ 7] = {175, 75, 37},
		[ 6] = {150,100, 50},
		[ 5] = {125,125, 62},
		[ 4] = {100,150, 75},
		[ 3] = { 87,175, 87},
		[ 2] = {100,200,100},
		[ 1] = {125,225,125}
	},
	.transparent = {[0] = 1},
	.w = GZ,
	.h = GZ,
	.cell_sz = SCALE
};

static void
vis22_begin(void)
{
	vis_begin(&vis, "day22.mp4", 60, gw*SCALE, gh*SCALE);
}

static void
vis22_emit(void)
{
	static int counter;
	int x,y;

	P[py][px] = '@';

	vis_draw_grid(&vis, &G_vis);
	vis_draw_grid(&vis, &P_vis);
	vis_emit(&vis, 1);

	if (++counter % 50 == 0) {
		for (y=0; y<gh; y++)
		for (x=0; x<gw; x++)
			if (P[y][x])
				P[y][x]--;
	}

	P[py][px] = 10;
}

static void
vis22_end(void)
{
	vis_end(&vis);
}
#else
static void vis22_begin(void) {}
static void vis22_emit(void) {}
static void vis22_end(void) {}
#endif
