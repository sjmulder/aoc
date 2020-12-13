#include <stdio.h>
#include <stdlib.h>

int
main()
{
	char c;
	int n,tmp, x=0,y=0, wx=10,wy=-1, rot;

	while(scanf(" %c%d", &c, &n) == 2) {
		switch (c) {
			case 'N': wy -= n; break;
			case 'S': wy += n; break;
			case 'W': wx -= n; break;
			case 'E': wx += n; break;
			case 'R': rot = n; break;
			case 'L': rot = 360-n; break;
			case 'F': x += wx*n; y += wy*n; break;
		}
		switch (rot) {
			case  90: tmp=wx; wx=-wy; wy= tmp; break;
			case 270: tmp=wx; wx= wy; wy=-tmp; break;
			case 180: wx=-wx; wy=-wy; break;
		}
		rot=0;
	}

	printf("%d\n", abs(x)+abs(y));
}
