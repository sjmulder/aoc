#include <stdio.h>

int
main()
{
	int tx0,tx1,ty0,ty1, vx0,vy0,vx,vy,x,y, p1,p2=0;

	scanf("target area: x=%d..%d, y=%d..%d", &tx0,&tx1,&ty0,&ty1);

	p1 = ty0 * (ty0+1) /2;

	for (vx0=1; vx0<=tx1; vx0++)
	for (vy0=ty0; vy0<-ty0; vy0++) {
		x=y=0; vx=vx0; vy=vy0;
		while ((x<tx0 || y>ty1) && x<=tx1 && y>=ty0) {
			x += vx; vx -= vx>0;
			y += vy; vy -= 1;
		}
		p2 += x>=tx0 && x<=tx1 && y>=ty0 && y<=ty1;
	}

	printf("17: %d %d\n", p1, p2);
}
