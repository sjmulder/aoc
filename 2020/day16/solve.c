#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

#define MAXFIELDS	32
#define MAXTICKETS	256
#define VERBOSE		0

struct rule { int min[2], max[2], isdep, field; };
struct ticket { int fs[MAXFIELDS], inval; };

static struct rule rules[MAXFIELDS];
static struct rule *rulemap[MAXFIELDS];
static struct ticket tickets[MAXTICKETS];
static int nfields, ntickets;

static void
parse(FILE *f)
{
	static char buf[128];
	struct rule *rule;
	struct ticket *ticket;
	char *c;
	int i, n;

	while (fgets(buf, sizeof(buf), f)) {
		if ((c = strchr(buf, '\n')))
			*c = '\0';
		if (!buf[0])
			break;	/* end of rules */
		assert(nfields < MAXFIELDS);
		rule = &rules[nfields++];
		rule->isdep = !!strstr(buf, "depart");
		n = sscanf(strchr(buf, ':'), ": %d-%d or %d-%d",
		    &rule->min[0], &rule->max[0],
		    &rule->min[1], &rule->max[1]);
		assert(n == 4);
	}

	while (1) {
		if (ntickets < 2)
			fgets(buf, sizeof(buf), f);
		if (ntickets == 1)
			fgets(buf, sizeof(buf), f);
		assert(nfields < MAXFIELDS);
		ticket = &tickets[ntickets];
		for (i=0; i < nfields; i++)
			if (fscanf(f, "%d", &ticket->fs[i]) == 1)
				fgetc(f);	/* , or \n */
			else if (i == 0)
				return;
			else
				assert(0 && "missing field");
		ntickets++;
	}
}

static int
isvalid(struct ticket *ticket, int field, struct rule *rule)
{
	return (ticket->fs[field] >= rule->min[0] &&
	        ticket->fs[field] <= rule->max[0]) ||
	       (ticket->fs[field] >= rule->min[1] &&
	        ticket->fs[field] <= rule->max[1]);
}

static int
testrule(struct rule *rule, int field)
{
	int i;

	for (i=1; i < ntickets; i++) {
		if (tickets[i].inval)
			continue;
		if (!isvalid(tickets+i, field, rule))
			return 0;
	}

	return 1;
}

int
main(int argc, char **argv)
{
	FILE *f;
	int ti,fi,ri, nassigned=0, nmatch, lastmatch=0;
	int64_t part1=0, part2=1;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	parse(f);

	for (ti=1; ti < ntickets; ti++)
	for (fi=0; fi < nfields; fi++) {
		for (ri=0; ri < nfields; ri++)
			if (isvalid(&tickets[ti], fi, &rules[ri]))
				goto anyvalid;
		tickets[ti].inval = 1;
		part1 += tickets[ti].fs[fi];
	anyvalid: ;
	}

	for (ri=0; ri < nfields; ri++)
		rules[ri].field = -1;

	while (nassigned < nfields)
		for (ri=0; ri < nfields; ri++) {
			if (rules[ri].field != -1)
				continue;
			nmatch = 0;
			for (fi=0; fi < nfields; fi++)
				if (!rulemap[fi] &&
				    testrule(&rules[ri], fi)) {
					nmatch++;
					lastmatch = fi;
				}
			if (VERBOSE)
				printf(" rule %d fits %d (last %d)\n",
				    ri, nmatch, lastmatch);
			if (nmatch == 1) {
				rules[ri].field = lastmatch;
				rulemap[lastmatch] = &rules[ri];
				nassigned++;
			}
		}

	for (ri=0; ri < nfields; ri++)
		if (rules[ri].isdep)
			part2 *= tickets[0].fs[rules[ri].field];

	printf("%" PRId64 " %" PRId64 "\n", part1, part2);
	//getchar();
	return 0;
}
