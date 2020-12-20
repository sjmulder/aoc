#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <err.h>

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

	while (*rule == ' ')
		rule++;
	if (!*rule || *rule == '|') {
		assert(maxendps > 0);
		endps[0] = s;
		return 1;
	}
	if (*rule == '"') {
		assert(maxendps > 0);
		endps[0] = s+1;
		return *s == rule[1];
	}

	assert(isdigit(*rule));

	id = (int)strtol(rule, &rule, 10);
	assert(id >= 0 && id < LEN(rules));
	assert(*rules[id]);

	nmatch = match(rules[id], s, endps, maxendps);
	nendps += nmatch;

	if (nmatch == 0)
		return 0;
	if (nmatch == 1) /* tail call */
		return matchsub(rule, endps[0], endps, maxendps);

	for (i=0; i<nmatch; i++)
		nendps += matchsub(rule, endps[i], endps+nendps,
		    maxendps-nendps);

	memmove(&endps[0], &endps[nmatch], (nendps-nmatch) * sizeof(*endps));
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
	char *pipe;

	while (1) {
		nendps += matchsub(rule, s, endps+nendps,
		    maxendps-nendps);
		if (!(pipe = strchr(rule, '|')))
			break;
		rule = pipe+1;
	}

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
	static char *endps[128];
	int i, nmatch, nfull=0;

	nmatch = match(rule, s, endps, LEN(endps));
	for (i=0; i<nmatch; i++)
		nfull += !*endps[i];

	return nfull;
}

int
main()
{
	static char buf[128];
	int id, count=0;
	char *cp;

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((cp = strchr(buf, '\n'))) *cp = '\0';
		if (!*buf) break;
		id = atoi(buf);
		cp = strchr(buf, ':');
		assert(cp);
		assert(strlen(cp+1) < LEN(*rules));
		snprintf(rules[id], LEN(*rules), "%s", cp+1);
	}

	assert(*rules[0]);

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((cp = strchr(buf, '\n'))) *cp = '\0';
		count += matchfull(rules[0], buf);
	}
	
	printf("%d\n", count);
}
