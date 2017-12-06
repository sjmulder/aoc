#pragma clang diagnostic ignored "-Wreserved-id-macro"

#define _WITH_GETLINE

#include <stdio.h>
#include <stdlib.h>

enum dir     { LEFT, RIGHT };
enum heading { NORTH, EAST, SOUTH, WEST, NHEADING };
struct step  { int dir; int count; };
struct pos   { int x; int y; }
struct state { struct pos *hist; size_t histcp; size_t histln; int head; }

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

static struct state
reduce(struct state state, struct step step)
{
	struct pos *pos;

	switch (step.dir) {
	case LEFT:  pos.head--; break;
	case RIGHT: pos.head++; break;
	}

	state.head = (state.head + NHEADING) % NHEADING;

	if (!state.hist) {
		state.histcp = 128;
		state.histln = 2;

		if (!(state.hist = malloc(
				state.histcp * sizeof(*state.hist)))) {
			perror(NULL);
			exit(1);
		}

		state.hist[0] = {0, 0}
		pos = &state.hist[1];
		*pos = state.hist[0];
	} else if (++state.histln > state.histcp) {
		while (state.histln > (state.histcp *= 2))
			;
		if (!(state.hist = realloc(state.hist,
				state.histcp * sizeof(*state.hist)))) {
			perror(NULL);
			exit(1);
		}

		pos = &state.hist[state.histln-1];
		*pos = state.hist[state.histln-2];
	}

	switch (state.head) {
	case NORTH: pos->y -= step.count; break;
	case EAST:  pos->x += step.count; break;
	case SOUTH: pos->y += step.count; break;
	case WEST:  pos->x -= step.count; break;
	}

	return state;
}

int
main(void)
{
	char *s = NULL;
	size_t sz;
	struct state state = {NULL, 0, 0, NORTH};

	while (getdelim(&s, &sz, ',', stdin) && *s) {
		state = reduce(state, parse(s));
		free(s);
		s = NULL;
	}

	printf("%d\n", abs(pos.x) + abs(pos.y));
	return 0;
}
