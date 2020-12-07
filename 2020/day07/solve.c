#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <err.h>

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

static char names[1024][32];
static struct { int subj, count, obj; } rules[4096];
static int nnames, nrules;

static int
getid(const char *name)
{
	int i;

	for (i=0; i<nnames; i++)
		if (!strcmp(names[i], name))
			return i;

	if (++nnames > LEN(names))
		errx(1, "name overflow");

	snprintf(names[i], LEN(names[i]), "%s", name);
	return i;
}

static void
parse(void)
{
	static char buf[256];
	char *c, *word, *rest;
	int subj, count, obj;

	while (fgets(buf, LEN(buf), stdin)) {
		if ((c = strchr(buf, '\n')))
			*c = '\0';

		word = rest = buf;
		rest = strchr(rest+1, ' '); assert(rest);
		rest = strchr(rest+1, ' '); assert(rest);
		*(rest++) = '\0';
		subj = getid(word);

		rest = strchr(rest+1, ' '); assert(rest);
		rest = strchr(rest+1, ' '); assert(rest);
		rest++;

		while (1) {
			word = rest;
			rest = strchr(rest+1, ' '); assert(rest);
			*(rest++) = '\0';
			if (!strcmp(word, "no"))
				break;
			count = atoi(word);

			word = rest;
			rest = strchr(rest+1, ' '); assert(rest);
			rest = strchr(rest+1, ' '); assert(rest);
			*(rest++) = '\0';
			obj = getid(word);

			if (nrules >= LEN(rules))
				errx(1, "rules overflow");
			rules[nrules].subj = subj;
			rules[nrules].count = count;
			rules[nrules].obj = obj;
			nrules++;

			rest = strchr(rest+1, ' ');
			if (!rest)
				break;
		}
	}
}

static int
holds(int subj, int obj)
{
	int i;

	for (i=0; i<nrules; i++) {
		if (rules[i].subj != subj)    continue;
		if (rules[i].obj == obj)      return 1;
		if (holds(rules[i].obj, obj)) return 1;
	}

	return 0;
}

static int
bagcount(int subj)
{
	int i, acc=1;

	for (i=0; i<nrules; i++)
		if (rules[i].subj == subj)
			acc += rules[i].count * bagcount(rules[i].obj);

	return acc;
}

int
main()
{
	int i, p1=0,p2, gold;

	parse();
	gold = getid("shiny gold");
	for (i=0; i<nnames; i++)
		p1 += holds(i, gold);
	p2 = bagcount(gold)-1;

	printf("%d %d\n", p1, p2);
}
