#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <err.h>

#define DATASZ (4 * 1024 * 1024)
#define MAPW 128

#define LINE(i) (&data[(i)*(linelen+1)])

/*
 * Task: find two lines that differ only by a single character; print the
 * matching characters.
 *
 * O(n) approach: add the lines to a hashmap twice, once keyed by the first
 * half of the line, once by the second. Since only one character may differ a
 * collision MUST occur on one of the halves. Then do a full compare only on
 * the collisions.
 *
 * Approach based on:
 * https://github.com/macdja38/advent-of-code-2018/blob/master/days/2-1/
 */

static unsigned shash(char *, size_t);
static int ndiff(char *, char *);
static void pcommon(char *, char *);

static char data[DATASZ];
static char *map[1<<20][MAPW];
static size_t linelen;
static size_t nlines;

int
main()
{
	size_t nread;
	size_t i, j, k;
	unsigned hashes[2];
	char *line, *ent;

	if ((nread = fread(data, 1, DATASZ, stdin)) == DATASZ)
		errx(1, "data overflow");
	linelen = strchr(data, '\n') - data;
	nlines = nread / linelen;

	for (i = 0; i < nlines; i++) {
		line = LINE(i);
		hashes[0] = shash(line, linelen/2) & 0xFFFFF;
		hashes[1] = shash(line + linelen/2, linelen/2) & 0xFFFFF;

		for (j = 0; j < 2; j++) {
			for (k = 0; k < MAPW; k++) {
				ent = map[hashes[j]][k];
				if (!ent) {
					map[hashes[j]][k] = line;
					break;
				} else if (ndiff(line, ent) == 1) {
					pcommon(line, ent);
					return 0;
				}
			}
			if (k == MAPW)
				errx(1, "map overflow");
		}
	}

	puts("no solution");
	return 1;
}

static unsigned
shash(char *s, size_t n)
{
	unsigned hash = 0;

	while (n)
		hash = (hash << 4) ^ s[--n];
	return hash;
}

/* number of different characters in a line, exits after 2 */
static int
ndiff(char *s1, char *s2)
{
	size_t i;
	int ndiff = 0;

	for (i = 0; i < linelen && ndiff < 2; i++)
		if (s1[i] != s2[i])
			ndiff++;
	return ndiff;
}

static void
pcommon(char *s1, char *s2)
{
	size_t i;

	for (i = 0; i < linelen; i++)
		if (s1[i] == s2[i])
			putchar(s1[i]);
	putchar('\n');
}
