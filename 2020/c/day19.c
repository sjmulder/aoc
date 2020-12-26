#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "compat/stdio.h"

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

static char rules[256][32];

static int match(char *, char *, char **, int);
static int matchsub(char *, char *, char **, int);

/*
 * Match a sub rule, that is, the things separated by | characters. Sub
 * rules have two forms:
 *
 *  "a"        -- match single character
 *  32 54 ...  -- match a series of rules
 *
 * The number of matches can be >1 because rules can match multiple of
 * its subrules. Those different subrules could cover different lengths
 * of the string, potentially leading to different outcomes for parent
 * rules, hence they all need to be considered.
 *
 * Returns number of matches, and writes pointers to the remainder of
 * s for those respective matches to endps.
 */
static int
matchsub(char *rule, char *s, char **endps, int maxendps)
{
	int i, nendps=0, id, nmatch;

	assert(maxendps > 0);

	while (*rule == ' ')
		rule++;
	if (!*rule || *rule == '|')
		{ endps[0] = s; return 1; }
	if (*rule == '"')
		{ endps[0] = s+1; return *s == rule[1]; }

	assert(isdigit(*rule));

	id = (int)strtol(rule, &rule, 10);
	assert(id >= 0 && id < LEN(rules));
	assert(*rules[id]);

	nendps = nmatch = match(rules[id], s, endps, maxendps);
	if (nmatch == 0)
		return 0;
	if (nmatch == 1) /* tail call */
		return matchsub(rule, endps[0], endps, maxendps);

	/*
	 * We have matched the first term, now recursively match the
	 * rest of the rule.
	 *
	 * Note that if we have 2 matches, and the continuations each
	 * yield 3, it's those 6 matches we should return. Hence our
	 * original matches in endps should be overwritten.
	 */
	for (i=0; i<nmatch; i++)
		nendps += matchsub(rule, endps[i], endps+nendps,
		    maxendps-nendps);

	memmove(&endps[0], &endps[nmatch], (nendps-nmatch) *
	   sizeof(*endps));
	return nendps-nmatch;
}

/*
 * Matches a full rule, e.g.  234 454 | 344 594
 *
 * Works like subrule() above with similar considerations.
 */
static int
match(char *rule, char *s, char **endps, int maxendps)
{
	int nendps = 0;

	do {
		nendps += matchsub(rule, s, endps+nendps,
		    maxendps-nendps);
		rule = strchr(rule, '|')+1;
	} while (rule != (char *)NULL+1);

	return nendps;
}

/*
 * Matches the _entire string_ to the given full rule, i.e. checks that
 * the rule covers the entire string.
 *
 * Returns number of matches.
 */
static int
matchfull(char *rule, char *s)
{
	static char *endps[16];
	int i, nmatch, nfull=0;

	nmatch = match(rule, s, endps, LEN(endps));
	for (i=0; i<nmatch; i++)
		nfull += !*endps[i];

	return nfull;
}

int
main(int argc, char **argv)
{
	static char buf[128];
	FILE *f;
	int id, count=0;
	char *cp;

	f = argc > 1 ? fopen(argv[1], "r") : stdin;
	assert(f);

	while (fgets(buf, sizeof(buf), f)) {
		if ((cp = strchr(buf, '\n'))) *cp = '\0';
		if (!*buf) break;
		id = atoi(buf);
		assert(id >= 0 && id < LEN(rules));
		cp = strchr(buf, ':');
		assert(cp);
		assert(strlen(cp+1) < LEN(*rules));
		snprintf(rules[id], LEN(*rules), "%s", cp+1);
	}

	assert(*rules[0]);

	while (fgets(buf, sizeof(buf), f)) {
		if ((cp = strchr(buf, '\n'))) *cp = '\0';
		count += matchfull(rules[0], buf);
	}
	
	printf("%d\n", count);
	//getchar();
	return 0;
}
