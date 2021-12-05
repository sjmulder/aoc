#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SIGN(x)	 ((x)<0?-1:(x)>0?1:0)
#define MAX(a,b) ((a)>(b)?(a):(b))

#define SZ 1000

int
main()
{
	static uint8_t map1[SZ][SZ], map2[SZ][SZ];
	int x1,y1,x2,y2, dx,dy,i,len, p1=0,p2=0;

	while (scanf(" %d,%d -> %d,%d", &x1, &y1, &x2, &y2) == 4) {
		dx = SIGN(x2-x1);
		dy = SIGN(y2-y1);
		len = MAX(abs(x2-x1), abs(y2-y1))+1;

		if (!dx || !dy)
			for (i=0; i<len; i++)
				p1 += ++map1[y1+dy*i][x1+dx*i] == 2;

		for (i=0; i<len; i++)
			p2 += ++map2[y1+dy*i][x1+dx*i] == 2;
	}

	printf("05: %d %d\n", p1, p2);
	return 0;
}
