#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

struct line {
	int x1, y1;
	int x2, y2;
	int len;
};

struct point {
	int x, y;
};

struct line paths[2][3*1024*1024];

static size_t read_path(struct line *, size_t);
static int intersects(struct line, struct line, struct point *);

int
main()
{
	size_t num_a, num_b;
	size_t a, b;
	int steps_a, steps_b;
	int steps, best_steps = INT_MAX;
	struct point pt;

	num_a = read_path(paths[0], LEN(paths[0]));
	num_b = read_path(paths[1], LEN(paths[1]));

#if 0
	/* output the occupied extent of the grid */

	int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
	for (a = 0; a < num_a; a++) {
		xmin = MIN(xmin, MIN(paths[0][a].x1, paths[0][a].x2));
		xmax = MAX(xmax, MAX(paths[0][a].x1, paths[0][a].x2));
		ymin = MIN(ymin, MIN(paths[0][a].y1, paths[0][a].y2));
		ymax = MAX(ymax, MAX(paths[0][a].y1, paths[0][a].y2));
	}
	for (b = 0; b < num_b; b++) {
		xmin = MIN(xmin, MIN(paths[1][b].x1, paths[1][b].x2));
		xmax = MAX(xmax, MAX(paths[1][b].x1, paths[1][b].x2));
		ymin = MIN(ymin, MIN(paths[1][b].y1, paths[1][b].y2));
		ymax = MAX(ymax, MAX(paths[1][b].y1, paths[1][b].y2));
	}
	fprintf(stderr, "xmin=%d xmax=%d ymin=%d ymax=%d\n", xmin, xmax,
	    ymin, ymax);
#endif

	steps_a = 0;
	for (a = 0; a < num_a; a++) {
		steps_b = 0;
		for (b = 1; b < num_b; b++) {
			if (a && b &&
			    intersects(paths[0][a], paths[1][b], &pt)) {
				steps = steps_a + steps_b +
				    abs(paths[0][a].x1 - pt.x) +
				    abs(paths[0][a].y1 - pt.y) +
				    abs(paths[1][b].x1 - pt.x) +
				    abs(paths[1][b].y1 - pt.y);
				if (steps < best_steps)
					best_steps = steps;
				break;
			}

			steps_b += paths[1][b].len;
			if (steps_a + steps_b >= best_steps)
				break;
		}

		steps_a += paths[0][a].len;
		if (steps_a >= best_steps)
			break;
	}

	if (best_steps == INT_MAX) {
		fprintf(stderr, " intersect\n");
		return 1;
	}

	printf("%d\n", best_steps);
	return 0;
}

static size_t
read_path(struct line *lines, size_t num)
{
	size_t i = 0;
	int x = 0, y = 0;
	char dir;
	int length;

	while (1) {
		if (scanf("%c%d", &dir, &length) != 2)
			errx(1, "failed to scan");
		if (i >= num)
			errx(1, "too many lines");

		lines[i].x1 = x;
		lines[i].y1 = y;
		lines[i].len = length;

		switch (dir) {
			case 'U': y -= length; break;
			case 'D': y += length; break;
			case 'L': x -= length; break;
			case 'R': x += length; break;
			default: errx(1, "unknown dir: %c", dir);
		}

		lines[i].x2 = x;
		lines[i].y2 = y;

		i++;

		switch (getchar()) {
			case ',': break;
			case '\n': return i;
			default: errx(1, "unexpected char");
		}
	}
}

static int
intersects(struct line a, struct line b, struct point *ptp)
{
	if (a.y1 == a.y2 && b.x1 == b.x2 &&
	    b.x1 >= MIN(a.x1, a.x2) && b.x1 <= MAX(a.x1, a.x2) &&
	    a.y1 >= MIN(b.y1, b.y2) && a.y1 <= MAX(b.y1, b.y2)) {
		ptp->x = b.x1;
		ptp->y = a.y1;
		return 1;
	}

	if (a.x1 == a.x2 && b.y1 == b.y2 &&
	    a.x1 >= MIN(b.x1, b.x2) && a.x1 <= MAX(b.x1, b.x2) &&
	    b.y1 >= MIN(a.y1, a.y2) && b.y1 <= MAX(a.y1, a.y2)) {
		ptp->x = a.x1;
		ptp->y = b.y1;
		return 1;
	}

	return 0;
}
