#include <stdio.h>

int owners[1024][1024];
int candidate[2048];

int main()
{
	int id, x0, y0, w, h;
	int x, y, i;

	while (scanf(" #%d @ %d,%d: %dx%d", &id, &x0, &y0, &w, &h) == 5) {
		candidate[id] = 1;
		for (x = x0; x < x0 + w; x++)
			for (y = y0; y < y0 + h; y++)
				if (owners[x][y]) {
					candidate[id] = 0;
					candidate[owners[x][y]] = 0;
				} else
					owners[x][y] = id;
	}

	for (i = 0; i < 2048; i++)
		if (candidate[i]) {
			printf("%d\n", i);
			return 0;
		}

	puts("no solution");
}
