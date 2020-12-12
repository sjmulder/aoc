#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main()
{
	char c;
	int n, x=0,y=0,dir=0;

	while(scanf(" %c%d", &c, &n) == 2) {
		if (c == 'F')
			c = "ESWN"[dir];
		switch (c) {
			case 'N': y -= n; break;
			case 'S': y += n; break;
			case 'W': x -= n; break;
			case 'E': x += n; break;
			case 'R': dir = (dir+  n/90) % 4; break;
			case 'L': dir = (dir+4-n/90) % 4; break;
		}
	}

	printf("%d\n", abs(x)+abs(y));
}
