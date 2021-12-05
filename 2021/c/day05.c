#include <stdio.h>
#include <stdint.h>

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

#define SZ 1000

int
main()
{
	static uint8_t map[SZ][SZ];
	int x1,y1, x2,y2, x,y, p1=0;

	while (scanf(" %d,%d -> %d,%d", &x1, &y1, &x2, &y2) == 4) {
		if (x1!=x2 && y1!=y2)
			continue;
		for (y=MIN(y1,y2); y<=MAX(y1,y2); y++)
		for (x=MIN(x1,x2); x<=MAX(x1,x2); x++)
			p1 += ++map[y][x] == 2;
	}

	printf("05: %d\n", p1);
	return 0;
}
