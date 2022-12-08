#include <stdio.h>
#include <assert.h>

#define SZ 128

static char heights[100][100];
static char visibility[100][100];
static int w,h, nvisible=0;

static void
vis_rays(
    int x0,  int y0,	/* start position */
    int dx0, int dy0,	/* start position step */
    int dx,  int dy)	/* ray step */
{
	int x1,y1, x,y, max_height;

	for (x1=x0, y1=y0;
	     x1>=0 && x1<w && y1>=0 && y1<h; 
	     x1+=dx0, y1+=dy0)
	for (x=x1, y=y1, max_height=0;
	     x>=0 && x<w && y>=0 && y<h; 
	     x+=dx, y+=dy)
		if (heights[y][x] > max_height) {
			max_height = heights[y][x];
			nvisible += !visibility[y][x];
			visibility[y][x] = 1;
		}
}

static int
dist_ray(
   int x0, int y0,
   int dx, int dy)
{
	int x,y, dist=0;

	for (x=x0+dx, y=y0+dy;
	     x>=0 && x<w && y>=0 && y<h;
	     x+=dx, y+=dy, dist++)
		if (heights[y][x] >= heights[y0][x0])
			return dist+1;

	return dist;
}

int
main()
{
	int len, x,y, scenic, max_scenic=0;

	for (; fgets(heights[h], SZ, stdin); h++) {
		for (len=0;
		     heights[h][len] && heights[h][len] != '\n';
		     len++) ;
		if (len > w) w = len;
		assert(h < SZ);
	}

	/* part 1 */
	vis_rays(0,h-1, 1,0, 0,-1); /* up */
	vis_rays(0,0,   1,0, 0, 1); /* down */
	vis_rays(w-1,0, 0,1, -1,0); /* left */
	vis_rays(0,0,   0,1,  1,0); /* right */

	/* part 2 */
	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		scenic  = dist_ray(x,y, 0,-1); /* up */
		scenic *= dist_ray(x,y, 0, 1); /* down */
		scenic *= dist_ray(x,y, -1,0); /* left */
		scenic *= dist_ray(x,y,  1,0); /* right */

		if (scenic > max_scenic)
			max_scenic = scenic;
	}

	printf("08: %d %d\n", nvisible, max_scenic);

	return 0;
}
