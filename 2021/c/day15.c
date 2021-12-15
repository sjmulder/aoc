#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))

#define SZ 500

int
main()
{
	static int costs[SZ][SZ];
	static int sums[SZ][SZ];
	int w=0,h=0, x=0,y=0, c, dirty=0, sum,cost;

	while ((c = getchar()) != EOF) {
		if (c == '\n') { x=0; y++; continue; }
		assert(x*5 < SZ);
		assert(y*5 < SZ);
		costs[y][x] = c-'0';
		w = MAX(w, x+1);
		h = MAX(h, y+1);
		x++;
	}

	for (y=0; y<h*5; y++)
	for (x=0; x<w*5; x++) {
		sums[y][x] = INT_MAX -10;
		costs[y][x] = (costs[y%h][x%w] + x/w + y/h -1) %9 +1;
	}

	sums[0][0] = 0;

	do {
		dirty=0;

		for (y=0; y<h*5; y++)
		for (x=0; x<w*5; x++) {
			sum = sums[y][x];
			cost = costs[y][x];

			if (y) sum = MIN(sum, sums[y-1][x]+cost);
			if (x) sum = MIN(sum, sums[y][x-1]+cost);
			if (y<h*5-1) sum = MIN(sum, sums[y+1][x]+cost);
			if (x<w*5-1) sum = MIN(sum, sums[y][x+1]+cost);

			if (sum != sums[y][x]) {
				//printf("  change in %d,%d\n", x,y);
				sums[y][x] = sum;
				dirty = 1;
			}
		}

#if 0
		printf("\n  sums\n");
		for (y=0; y<h*5; y++) {
			for (x=0; x<w*5; x++)
				printf("%3d ", sums[y][x]);
			putchar('\n');
		}
#endif
	} while (dirty);

#if 0
	printf("costs\n");
	for (y=0; y<h; y++) {
		for (x=0; x<w; x++)
			printf("%3d ", costs[y][x]);
		putchar('\n');
	}
#endif

	printf("14: %d %d\n", sums[h-1][w-1], sums[h*5-1][w*5-1]);
	return 0;
}
