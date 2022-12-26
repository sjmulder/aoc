#include "common.h"

static int sign(int x) { return x>0 ? 1 : x<0 ? -1 : 0; }

int
main(int argc, char **argv)
{
	static char visited[512][512];
	char dir;
	int x[10],y[10], p1=0,p2=0, dx,dy, steps, i;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);
	for (i=0; i<10; i++)
		x[i] = y[i] = 256;

	while (scanf(" %c %d", &dir, &steps) == 2)
	while (steps-- > 0) {
		switch (dir) {
		case 'U': y[0]--; assert(x[0] >=  0); break;
		case 'D': y[0]++; assert(x[0] < 512); break;
		case 'L': x[0]--; assert(y[0] >=  0); break;
		case 'R': x[0]++; assert(y[0] < 512); break;
		}
		 
		for (i=1; i<10; i++) {
			dx = x[i-1] - x[i];
			dy = y[i-1] - y[i];

			if (abs(dx) <= 1 && abs(dy) <= 1)
				break; /* rest of rope doesn't move */

			x[i] += sign(dx);
			y[i] += sign(dy);
		}

		if (!(visited[y[1]][x[1]] & 1))
			{ visited[y[1]][x[1]] |= 1; p1++; }
		if (!(visited[y[9]][x[9]] & 2))
			{ visited[y[9]][x[9]] |= 2; p2++; }
	}

	printf("09: %d %d\n", p1, p2);
	return 0;
}
