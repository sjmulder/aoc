#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <err.h>

/*
 * Attempt at using a steps bitmap. Even when cheating and using the exact
 * grid dimensions of the solution the bitmap is large enough to segfault
 * my Pi. On my PC it works but is slower than the real thing and segfaults
 * when using a grid large enough for input.big.
 */

#define XMIN (-6364)
#define XMAX 18409
#define YMIN (-6059)
#define YMAX 6339

#define GRIDW (-XMIN+XMAX+1)
#define GRIDH (-YMIN+YMAX+1)

#define STEPS_MAX UINT32_MAX

typedef uint32_t Steps;

/* cumulative steps per cell for first path */
static Steps path0[GRIDW][GRIDH];

int
main()
{
	int x, y, x2, y2, dx, dy;
	int path_idx = 0;
	Steps steps = 0; /* cumulative in current path */
	char dir;
	int length;
	Steps candidate = STEPS_MAX;

	x = -XMIN;
	y = -YMIN;

	while (1) {
		if (scanf("%c%d", &dir, &length) != 2)
			errx(1, "failed to scan");

		x2 = x; y2 = y;
		dx = 0; dy = 0;
		switch (dir) {
			case 'U': y2 -= length; dy = -1; break;
			case 'D': y2 += length; dy =  1; break;
			case 'L': x2 -= length; dx = -1; break;
			case 'R': x2 += length; dx =  1; break;
			default: errx(1, "unknown dir: %c", dir);
		}

		if (x2 < 0 || x2 >= GRIDW || y2 < 0 || y2 >= GRIDH)
			errx(1, "out of bounds");

		while (x != x2 || y != y2) {
			if (steps == STEPS_MAX)
				errx(1, "steps overflow");

			x += dx; y += dy; steps++;

			if (path_idx) {
				if (steps > candidate)
					goto skip_rest;
				/* second path, check for hit */
				if (path0[x][y] &&
				    path0[x][y] + steps < candidate)
					candidate = path0[x][y] + steps;
			} else {
				/* first path, store */	
				if (!path0[x][y])
					path0[x][y] = steps;
			}
		}

		switch (getchar()) {
		case ',':
			break;
		case '\n':
			if (++path_idx > 1)
				goto skip_rest;
			steps = 0;
			x = -XMIN;
			y = -YMIN;
			break;
		default:
			errx(1, "unexpected char");
		}
	}

skip_rest:
	if (candidate == STEPS_MAX) {
		fprintf(stderr, "no solution\n");
		return 1;
	}

	printf("%d\n", (int)candidate);
	return 0;
}
