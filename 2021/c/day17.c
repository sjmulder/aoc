#include <stdio.h>
#include <math.h>

int
main()
{
	int l,r,b,t, vx0,vy0,vx,vy,x,y, p1,p2=0;

	scanf("target area: x=%d..%d, y=%d..%d", &l,&r,&b,&t);

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
