#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <err.h>

#define LEN(x) ((int)(sizeof(x)/sizeof(*(x))))

#define VERBOSE 0

static char names[64][16];
static int prefs[LEN(names)][LEN(names)];
static int nnam;

static int
nameidx(char *name)
{
	int i;

	for (i=0; i<nnam; i++)
		if (!strcmp(name, names[i]))
			return i;

	if (++nnam >= LEN(names)) /* leave 1 for part 2 */
		errx(1, "name overflow");

	snprintf(names[i], LEN(names[i]), "%s", name);
	return i;
}

static void
parse(void)
{
	char who[16], whom[16], what[5];
	int n, pts;

	while (1) {
		n = scanf(" %15s would %4s %d happiness units by "
		    "sitting next to %15s", who, what, &pts, whom);
		if (n != 4)
			break;
		if (!strcmp(what, "lose"))
			pts *= -1;

		whom[strlen(whom)-1] = '\0';
		prefs[nameidx(who)][nameidx(whom)] = pts;
	}
}

static void
dump(void)
{
	int i,j;

	for (i=0; i<nnam; i++)
	for (j=0; j<nnam; j++)
		printf("%s %s %d\n", names[i], names[j], prefs[i][j]);
}

static int
solve(void)
{
	static int seats[LEN(names)];
	int i,j, best=INT_MIN, score;

	while (1) {
		for (i = nnam-1; i>=0 && seats[i] == nnam-1; i--)
			;
		if (i == -1)
			break;
		seats[i]++;
		for (i = i+1; i<nnam; i++)
			seats[i] = 0;

		if (VERBOSE)
			for (i=0; i<nnam; i++)
				printf("%s\t", names[seats[i]]);

		for (i=0; i<nnam; i++)
		for (j=0; j<i; j++)
			if (seats[i] == seats[j]) {
				if (VERBOSE) puts("");
				goto end;
			}

		score = 0;
		for (i=0; i < nnam; i++) {
			score += prefs[seats[i]][seats[(i+1) % nnam]];
			score += prefs[seats[(i+1) % nnam]][seats[i]];
		}

		if (VERBOSE)
			printf("%d\n", score);
		if (score > best)
			best = score;
	end: ;
	}

	return best;
}

int
main()
{
	int p1, p2;

	parse();
	if (VERBOSE) dump();

	p1 = solve(); nnam++;
	p2 = solve(); 

	printf("13: %d %d\n", p1, p2);
	return 0;
}
