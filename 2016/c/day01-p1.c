#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _WITH_GETLINE

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <stdio.h>
#include <stdlib.h>

/* TODO: outputs 'invalid input' */

enum dir     { LEFT, RIGHT };
enum heading { NORTH, EAST, SOUTH, WEST, NHEADING };
struct step  { int dir; int count; };
struct pos   { int x; int y; int head; };

static struct step
parse(char *s)
{
	struct step step;
	char dirc;

	if (sscanf(s, " %c%d", &dirc, &step.count) != 2) {
		fprintf(stderr, "invalid input: '%s'\n", s);
		exit(1);
	}

	switch (dirc) {
	case 'L': step.dir = LEFT; break;
	case 'R': step.dir = RIGHT; break;

	default:
		fprintf(stderr, "invalid direction: '%c' in '%s'\n", dirc, s);
		exit(1);
	}

	return step;
}

static struct pos
reduce(struct pos pos, struct step step)
{
	switch (step.dir) {
	case LEFT:  pos.head--; break;
	case RIGHT: pos.head++; break;
	}

	pos.head = (pos.head + NHEADING) % NHEADING;

	switch (pos.head) {
	case NORTH: pos.y -= step.count; break;
	case EAST:  pos.x += step.count; break;
	case SOUTH: pos.y += step.count; break;
	case WEST:  pos.x -= step.count; break;
	}

	return pos;
}

int
main(void)
{
	char *s = NULL;
	size_t sz;
	struct pos pos = {0, 0, NORTH};

	while (getdelim(&s, &sz, ',', stdin) && *s) {
		pos = reduce(pos, parse(s));
		free(s);
		s = NULL;
	}

	printf("%d\n", abs(pos.x) + abs(pos.y));
	return 0;
}
