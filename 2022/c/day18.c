#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WITH_VIS
# include "vis.h"
#endif

#define GSZ	24

enum { AIR, CUBE, EXPOSED };

static uint8_t grid[GSZ][GSZ][GSZ];

static int oob(int n) { return n<0 || n>=GSZ; }
static int oob3(int x, int y, int z) { return oob(x)||oob(y)||oob(z); }

#ifdef WITH_VIS
static struct vis_grid vis_grid = {
	.w = GSZ,
	.h = GSZ,
	.cell_sz = 16,
	.border = 1,
	.colors = {
		[AIR] = {64,64,196},
		[CUBE] = {128,128,128}
	},
	.transparent = {
		[EXPOSED] = 1
	}
};

static void
vis18_render(void)
{
	struct vis vis;
	size_t z, i;

	vis_begin(&vis, "day18.mp4", 30,
	    GSZ * vis_grid.cell_sz + (GSZ-1)*8,
	    GSZ * vis_grid.cell_sz + (GSZ-1)*8);

	vis_grid.x = (GSZ-1)*8;
	vis_grid.y = 0;

	for (z=0; z<GSZ; z++) {
		vis_grid.x -= 8;
		vis_grid.y += 8;

		vis_grid.data = (uint8_t *)grid[z];
		vis_draw_grid(&vis, &vis_grid);
		vis_emit(&vis, z == GSZ-1 ? 30 : 15);

		for (i=0; i < vis.w * vis.h *3; i++)
			vis.frame[i] = (uint8_t)(2* vis.frame[i] /3);
	}

	vis_end(&vis);
}
#endif

int
main()
{
	int p1=0,p2=0, x,y,z, u,v,w, nchanged=0;

	while (scanf(" %d,%d,%d", &x,&y,&z) == 3) {
		assert(!oob3(x,y,z));
		grid[x][y][z] = CUBE;
	}

	do {
		nchanged = 0;

		for (x=0; x<GSZ; x++)
		for (y=0; y<GSZ; y++)
		for (z=0; z<GSZ; z++)
		for (u=x-1; u<=x+1; u++)
		for (v=y-1; v<=y+1; v++)
		for (w=z-1; w<=z+1; w++) {
			if (grid[x][y][z] != AIR ||
			    abs(x-u) + abs(y-v) + abs(z-w) != 1)
				continue;
			if (oob3(u,v,w) || grid[u][v][w] == EXPOSED) {
				grid[x][y][z] = EXPOSED;
				nchanged++;
			}
		}
	} while (nchanged);

	for (x=0; x<GSZ; x++)
	for (y=0; y<GSZ; y++)
	for (z=0; z<GSZ; z++)
	for (u=x-1; u<=x+1; u++)
	for (v=y-1; v<=y+1; v++)
	for (w=z-1; w<=z+1; w++) {
		if (grid[x][y][z] != CUBE ||
		    abs(x-u) + abs(y-v) + abs(z-w) != 1)
				continue;
		p1 += oob3(u,v,w) || grid[u][v][w] != CUBE;
		p2 += oob3(u,v,w) || grid[u][v][w] == EXPOSED;
	}

#if WITH_VIS
	vis18_render();
#endif

	printf("18: %d %d\n", p1, p2);
	return 0;
}
