#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "compat/stdio.h"
#include "compat/string.h"

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

static char names[1024][32];
static int counts[1024][1024];
static int nnames;

static int
getid(const char *name)
{
	int i;

	for (i=0; i<nnames; i++)
		if (!strcmp(names[i], name))
			return i;
	
	nnames++;
	assert(nnames <= LEN(names));
	snprintf(names[i], LEN(names[i]), "%s", name);

	return i;
}

static void
parse(FILE *f)
{
	char adj[16], col[16], name[32];
	int c=0, subj, count;

	while (c != EOF) {
		if (fscanf(f, "%15s %15s", adj, col) != 2)
			return;
		snprintf(name, 32, "%s %s", adj, col);
		subj = getid(name);

		while ((c=fgetc(f)) != EOF && c!='\n')
			if (isdigit(c))
				{ ungetc(c, f); break; }
		while (c != EOF && c != '\n') {
			if (fscanf(f, " %d %15s %15s", &count,adj,col)
			    != 3)
				return;
			snprintf(name, 32, "%s %s", adj, col);
			counts[subj][getid(name)] = count;

			while ((c=fgetc(f))!=EOF && c!=',' && c!='\n')
				;
		}
	}
}

static int
holds(int subj, int obj)
{
	int i;

	if (counts[subj][obj])
		return 1;
	for (i=0; i<nnames; i++)
		if (counts[subj][i] && holds(i, obj))
			return 1;
	return 0;
}

static int
bagcount(int subj)
{
	int i, acc=1;

	for (i=0; i<nnames; i++)
		if (counts[subj][i])
			acc += counts[subj][i] * bagcount(i);
	return acc;
}

int
main(int argc, char **argv)
{
	FILE *f;
	int i, p1=0,p2, gold;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	parse(f);
	gold = getid("shiny gold");
	for (i=0; i<nnames; i++)
		p1 += holds(i, gold);
	p2 = bagcount(gold)-1;

	printf("%d %d\n", p1, p2);
	//getchar();
	return 0;
}
