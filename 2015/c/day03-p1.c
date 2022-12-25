#include <stdio.h>
#include <err.h>

#define GSZ		(16*1024)
#define MAX(a,b)	((a)>(b)?(a):(b))

int
main()
{
	static int g[GSZ][GSZ];
	int c, x=GSZ/2, y=GSZ/2, p1=1;

	g[y][x] = 1;

	while ((c = getchar()) != EOF) {
		     if (c == '^') --y; else if (c == 'v') ++y;
		else if (c == '<') --x; else if (c == '>') ++x;
		else continue;

		if (x<0 || y<0 || x>=GSZ || y>=GSZ)
			errx(1, "overflow");

		p1 += ++g[y][x] == 1;
	}

	printf("%d\n", p1);
	return 0;
}
