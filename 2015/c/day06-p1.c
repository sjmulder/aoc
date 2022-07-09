#include <stdio.h>
#include <string.h>

enum {ON, OFF, TGL};

int
main()
{
	static char g[1000][1000];
	char b[8];
	int op, x0,x1,y0,y1, x,y, p1=0;

	while (scanf(" %7s", b) == 1) {
		if (!strcmp(b, "toggle"))
			op = TGL;
		else {
			scanf(" %7s", b);
			op = !strcmp(b, "on") ? ON : OFF;
		}

		scanf(" %d,%d %7s %d,%d", &x0, &y0, b, &x1, &y1);

		for (x=x0; x<=x1; x++)
		for (y=y0; y<=y1; y++)
			switch (op) {
				case ON : g[y][x] = 1; break;
				case OFF: g[y][x] = 0; break;
				case TGL: g[y][x] = !g[y][x]; break;
			}
	}

	for (x=0; x<1000; x++)
	for (y=0; y<1000; y++)
		p1 += g[y][x];

	printf("%d\n", p1);
}
