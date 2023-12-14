#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

int
main()
{
	char line[80], *t, *rest;
	int xs[8],ys[8], n, x,y, cx,cy, i;
	int xmin,ymin, xmax,ymax, sqmax, minsqmax=INT_MAX;
	double p1=0, p2=0;

	while ((rest = fgets(line, sizeof(line), stdin))) {
		xmin = ymin = INT_MAX;
		xmax = ymax = INT_MIN;
		sqmax = n = 0;

		while ((t = strsep(&rest, "("))) {
			if (sscanf(t, "%d, %d", &x, &y) != 2) continue;
			xs[n++] = x; ys[n] = y; assert(n < 8);
			if (x < xmin) xmin = x; if (x > xmax) xmax = x;
			if (y < ymin) ymin = y; if (y > ymax) ymax = y;
			if (x*x + y*y > sqmax) sqmax = x*x + y*y;
		}

		p1 += sqrt(sqmax);

		for (cx = xmin; cx <= xmax; cx++)
		for (cy = ymin; cy <= ymax; cy++) {
			printf(" at %d,%d\n", cx, cy);
			for (sqmax=0, i=0; i<n; i++) {
				x = xs[i] - cx;
				y = ys[i] - cy;
				printf("  x=%d-%d=%d y=%d-%d=%d sq=%d\n",
				    xs[i], cx, x,
				    ys[i], cy, y,
				    x*x + y*y);
				if (x*x + y*y > sqmax) {
					printf("   new max\n");
					sqmax = x*x + y*y;
				}
			}
			if (sqmax < minsqmax) {
				printf(" new min at %d,%d: %d\n", cx,cy,
				    sqmax);
				minsqmax = sqmax;
			}
		}

		p2 += sqrt(minsqmax);
	}

	printf("%d %d\n", (int)p1, (int)p2);
	return 0;
}
