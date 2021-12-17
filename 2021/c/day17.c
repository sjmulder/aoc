#include <stdio.h>
#include <math.h>

int
main()
{
	int left,right,bottom,top, vx0,vy0,vx,vy,x,y, p1,p2=0;

	scanf("target area: x=%d..%d, y=%d..%d",
	    &left, &right, &bottom, &top);

	p1 = bottom * (bottom+1) /2;

	for (vx0 = (int)sqrt(left)/2; vx0 <= right; vx0++)
	for (vy0 = bottom; vy0 < -bottom; vy0++) {
		x=y=0; vx=vx0; vy=vy0;
		while ((x<left || y>top) && x<=right && y>=bottom) {
			x += vx; vx -= vx>0;
			y += vy; vy -= 1;
		}
		p2 += x>=left && x<=right && y>=bottom && y<=top;
	}

	printf("17: %d %d\n", p1, p2);
	return 0;
}
