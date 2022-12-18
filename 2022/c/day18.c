#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define GSZ	24

enum { AIR, CUBE, EXPOSED };

static int oob(int n) { return n<0 || n>=GSZ; }
static int oob3(int x, int y, int z) { return oob(x)||oob(y)||oob(z); }

int
main()
{
	static uint8_t grid[GSZ][GSZ][GSZ];
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

	printf("18: %d %d\n", p1, p2);
	return 0;
}
