#include <stdio.h>
#include <err.h>

#define GSZ		(16*1024)
#define MAX(a,b)	((a)>(b)?(a):(b))

int
main()
{
	static int g[GSZ][GSZ];
	int c, x[2], y[2], i=0, p2=1;

	x[0] = x[1] = y[0] = y[1] = GSZ/2;
	g[GSZ/2][GSZ/2] = 1;

	while ((c = getchar()) != EOF) {
		     if (c == '^') --y[i]; else if (c == 'v') ++y[i];
		else if (c == '<') --x[i]; else if (c == '>') ++x[i];
		else continue;

		if (x[i]<0 || y[i]<0 || x[i]>=GSZ || y[i]>=GSZ)
			errx(1, "overflow");

		p2 += ++g[y[i]][x[i]] == 1;
		i = !i;
	}

	printf("%d\n", p2);
	return 0;
}
