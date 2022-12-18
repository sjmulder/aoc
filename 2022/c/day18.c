#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define GSZ	24
#define SHIFT	1

int
main()
{
	static char grid[GSZ][GSZ][GSZ];
	int x,y,z, dx,dy,dz, p1=0;

	while (scanf(" %d,%d,%d", &x,&y,&z) == 3) {
		x+=SHIFT; y+=SHIFT; z+=SHIFT;

		assert(x>0); assert(x < GSZ-1);
		assert(y>0); assert(y < GSZ-1);
		assert(z>0); assert(z < GSZ-1);

		grid[z][y][x] = 1;
		p1 += 6;

		for (dz=-1; dz<=1; dz++)
		for (dy=-1; dy<=1; dy++)
		for (dx=-1; dx<=1; dx++)
			if (abs(dx)+abs(dy)+abs(dz) == 1 &&
			    grid[z+dz][y+dy][x+dx])
				p1 -= 2;
	}

	printf("18: %d\n", p1);
	return 0;
}
