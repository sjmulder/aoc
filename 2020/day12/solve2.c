#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main()
{
	char c;
	int n, x=0,y=0, wx=10,wy=-1, rot, wx0,wy0;

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
		wx0=wx; wy0=wy;
		switch (rot) {
			case  90: wx=-wy0; wy= wx0; break;
			case 180: wx=-wx0; wy=-wy0; break;
			case 270: wx= wy0; wy=-wx0; break;
		}
		rot=0;
	}

	printf("%d\n", abs(x)+abs(y));
}
