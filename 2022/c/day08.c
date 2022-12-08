#include <stdio.h>
#include <assert.h>

#define SZ 128

static char heights[SZ][SZ];
static int w,h;

static int
dist_ray(int x0, int y0, int dx, int dy, int *oob)
{
	int x,y, dist=0;

	for (x=x0+dx, y=y0+dy;
	     x>=0 && x<w && y>=0 && y<h;
	     x+=dx, y+=dy, dist++)
		if (heights[y][x] >= heights[y0][x0])
			return dist+1;

	*oob = 1;
	return dist;
}

int
main()
{
	int x,y, len;
	int max_scenic=0, scenic;
	int nvisible=0, oob;

	for (; fgets(heights[h], SZ, stdin); h++) {
		for (len=0;
		     heights[h][len] && heights[h][len] != '\n';
		     len++) ;
		if (len > w) w = len;
		assert(h < SZ);
	}

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		oob = 0;
		scenic  = dist_ray(x,y, 0,-1, &oob); /* up */
		scenic *= dist_ray(x,y, 0, 1, &oob); /* down */
		scenic *= dist_ray(x,y, -1,0, &oob); /* left */
		scenic *= dist_ray(x,y,  1,0, &oob); /* right */

		if (scenic > max_scenic)
			max_scenic = scenic;
		if (oob)
			nvisible++;
	}

	printf("08: %d %d\n", nvisible, max_scenic);
	return 0;
}
