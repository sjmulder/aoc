#include "common.h"

static char dirs[512];
static char names[1024][4];
static size_t map[1024][2];	/* left and right */
static size_t nnames; 

static ssize_t
get_name_idx(const char *name)
{
	size_t i;

	for (i=0; i<nnames; i++)
		if (!strcmp(names[i], name))
			return i;

	assert(nnames < LEN(names));
	snprintf(names[nnames], LEN(*names), "%s", name);
	return nnames++;
}

/* returns no. steps to first terminal */
static int
count_steps(size_t pos, int part)
{
	int step=0;
	char *dir;

	for (dir=dirs; ; dir++, step++) {
		if ((!part && !strcmp(names[pos], "ZZZ")) ||
		    ( part && names[pos][2] == 'Z'))
			return step;
		if (*dir != 'R' && *dir != 'L')
			dir = dirs;
		pos = map[pos][*dir == 'R'];
	}
}

int
main(int argc, char **argv)
{
	char name[4], left[4], right[4];
	size_t pos;
	int64_t p1=0,p2=0, steps,inc;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	fgets(dirs, sizeof(dirs), stdin);

	while (scanf(" %3s = (%3s, %3s)", name, left, right) == 3) {
		pos = get_name_idx(name);
		map[pos][0] = get_name_idx(left);
		map[pos][1] = get_name_idx(right);
	}

	p1 = count_steps(get_name_idx("AAA"), 0);

	/*
	 * Part 2. Using LCM assuming that:
	 *  - all routes are cyclic
	 *  - exactly 1 terminal is visited each cycle
	 *  - period = first terminal, ie: terminals at 0(mod period)
	 */
	for (pos=0; pos<nnames; pos++)
		if (names[pos][2] == 'A') {
			steps = count_steps(pos, 1);
			if (!p2)
				p2 = steps;
			else for (inc = p2; p2 % steps; p2 += inc)
				;
		}

	printf("08: %"PRIi64" %"PRIi64"\n", p1, p2);
	return 0;
}
