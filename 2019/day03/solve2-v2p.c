/*
 * THe worst attempt at a parallel implementation - it performs terrible and
 * I think that's because the later jobs fail to do early exits (on the
 * steps vs. current best check) because they're not aware of the much much
 * lower current best the other job found.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define NJOBS 4

struct line {
	int x1, y1;
	int x2, y2;
	int len;
};

struct point {
	int x, y;
};

struct line paths[2][3*1024*1024];
static size_t num_a, num_b;

static int solve_part(size_t, size_t);
static size_t read_path(struct line *, size_t);
static int intersects(struct line, struct line, struct point *);

int
main()
{
	int i, best = INT_MAX, job_best;
	int report_fds[2], status;
	ssize_t ssz;

	num_a = read_path(paths[0], LEN(paths[0]));
	num_b = read_path(paths[1], LEN(paths[1]));

	if (pipe(report_fds) == -1)
		err(1, "pipe");

	for (i = 0; i < NJOBS; i++) {
		fprintf(stderr, "job %d starting\n", i);
		switch (fork()) {
		case -1: err(1, "fork");
		case 0:
			fprintf(stderr, "job %d started\n", i);
			job_best = solve_part(
			    (num_a/NJOBS + 1) * i,
			    MIN(num_a, (num_a/NJOBS + 1) * (i+1)));
			ssz = write(report_fds[1], &job_best,
			    sizeof(job_best));
			if (ssz == -1)
				err(1, "write");
			fprintf(stderr, "job %d finished\n", i);
			exit(0);
		}
	}

	for (i = 0; i < NJOBS; i++) {
		ssz = read(report_fds[0], &job_best, sizeof(job_best));
		if (ssz == -1)
			err(1, "read");
		if (job_best < best)
			best = job_best;
	}

	while (wait(&status) != -1)
		if (status)
			errx(1, "child failed");
	if (errno != ECHILD)
		err(1, "wait");

	if (best == INT_MAX) {
		fprintf(stderr, "no solution\n");
		return 1;
	}

	printf("%d\n", best);
	return 0;
}

static int
solve_part(size_t a_start, size_t a_end)
{
	int steps_a, steps_b;
	int steps, best_steps = INT_MAX;
	size_t a, b;
	struct point pt;

	fprintf(stderr, "part: %zu-%zu\n", a_start, a_end);

	steps_a = 0;
	for (a = 0; a < a_start; a++)
		steps_a += paths[0][a].len;

	for (a = a_start; a < a_end; a++) {
		if (!(a % 1000))
			fprintf(stderr, "%zu\n", a);
		steps_b = 0;
		for (b = 0; b < num_b; b++) {
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

	return best_steps;
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
