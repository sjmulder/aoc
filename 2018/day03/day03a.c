#include <stdio.h>

int nclaims[1024][1024];

int main()
{
	int id, x0, y0, w, h;
	int x, y;
	int noverlap = 0;

	while (scanf(" #%d @ %d,%d: %dx%d", &id, &x0, &y0, &w, &h) == 5)
		for (x = x0; x < x0 + w; x++)
			for (y = y0; y < y0 + h; y++)
				nclaims[x][y]++;

	for (x = 0; x < 1024; x++)
		for (y = 0; y < 1024; y++)
			if (nclaims[x][y] > 1)
				noverlap++;

	printf("%d\n", noverlap);
}
