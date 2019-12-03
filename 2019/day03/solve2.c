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
};

struct point {
	int x, y;
};

struct line paths[2][1024];

static size_t read_path(struct line *, size_t);
static int intersects(struct line, struct line, struct point *);
static int count_steps(struct line *, struct line *, struct point);

int
main()
{
	size_t a_len, b_len;
	int i, j;
	int steps, best_steps = INT_MAX;
	struct point pt;

	a_len = read_path(paths[0], 1024);
	b_len = read_path(paths[1], 1024);

	for (i = 1; i < (int)a_len; i++)
	for (j = 1; j < (int)b_len; j++) {
		if (!intersects(paths[0][i], paths[1][j], &pt))
			continue;
		steps  = count_steps(&paths[0][i], paths[0], pt);
		steps += count_steps(&paths[1][j], paths[1], pt);
		if (steps < best_steps)
			best_steps = steps;
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
	struct line *horz, *vert;

	if (a.y1 == a.y2) {
		if (b.y1 == b.y2)
			return 0;
		if (b.x1 != b.x2)
			errx(1, "line b is diagonal");
		horz = &a;
		vert = &b;
	} else if (a.x1 == a.x2) {
		if (b.x1 == b.x2)
			return 0;
		if (b.y1 != b.y2)
			errx(1, "line b is diagonal");
		horz = &b;
		vert = &a;
	} else
		errx(1, "line a is diagonal");

	if (vert->x1 < MIN(horz->x1, horz->x2) ||
	    vert->x1 > MAX(horz->x1, horz->x2) ||
	    horz->y1 < MIN(vert->y1, vert->y2) ||
	    horz->y1 > MAX(vert->y1, vert->y2))
		return 0;

	if (ptp) {
		ptp->x = vert->x1;
		ptp->y = horz->y1;
	}
	return 1;
}

static int
count_steps(struct line *line, struct line *path, struct point pt)
{
	int i, steps = 0;

	for (i = 0; line != &path[i]; i++) {
		steps += abs(path[i].x1 - path[i].x2);
		steps += abs(path[i].y1 - path[i].y2);
	}

	steps += abs(line->x1 - pt.x);
	steps += abs(line->y1 - pt.y);

	return steps;
}
