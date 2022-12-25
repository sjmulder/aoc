#include <stdio.h>
#include <math.h>
#include <assert.h>

/*
 * Given my terrible mathematical insights I was quite proud to have
 * figured out that, for part 1:
 *
 *  - the probe will always return to y=0 at which point the downward
 *    velocity exactly matches the initial upward velocity,
 *  - from y=0, the target can only be hit if the velocity is less than
 *    the distance to the bottom edge of the target,
 *  - hence the maximum initial upward velocity is the depth of the
 *    target bottom edge.
 *
 * This also sets some reasonable limits for a brute force search for
 * part 2 but I was frustrated not to find a more elegant solution for
 * that.
 */

int
main()
{
	int l,r,b,t, vx0,vy0,vx,vy,x,y, p1,p2=0;

	if (scanf("target area: x=%d..%d, y=%d..%d", &l,&r,&b,&t) != 4)
		assert(!"bad input");

	p1 = b * (b+1) /2;

	for (vx0 = (int)sqrt(l*2); vx0 <= r; vx0++) {
		for (x=0, vx=vx0; x<l && vx; x+=vx, vx--)
			;
		if (x<l || x>r)
			continue;
		for (vy0=b; vy0<-b; vy0++) {
			x=y=0; vx=vx0; vy=vy0;
			while ((x<l || y>t) && x<=r && y>=b) {
				x += vx; vx -= vx>0;
				y += vy; vy -= 1;
			}
			p2 += x>=l && x<=r && y>=b && y<=t;
		}
	}

	printf("17: %d %d\n", p1, p2);
	return 0;
}
