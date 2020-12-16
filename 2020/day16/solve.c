#include <stdio.h>
#include <string.h>
#include <err.h>

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
dump(void)
{
#if VERBOSE
	int i, j;

	printf("___\nrules:\n");
	for (i=0; i < nfields; i++)
		printf(" [%d]->[%d] %d-%d, %d-%d %s\n",
		    i, rules[i].field,
		    rules[i].min[0], rules[i].max[0],
		    rules[i].min[1], rules[i].max[1],
		    rules[i].isdep ? "DEP" : "");

	printf("\ntickets:\n");
	for (i=0; i < ntickets; i++) {
		printf(" [%d] ", i);
		for (j=0; j < nfields-1; j++)
			printf("%d,", tickets[i].fs[j]);
		printf("%d %s\n", tickets[i].fs[j],
		    tickets[i].inval ? "BAD" : "");
	}

	putchar('\n');
#endif
}

static void
parse(void)
{
	static char buf[128];
	struct rule *rule;
	struct ticket *ticket;
	char *c;
	int i;

	while (fgets(buf, sizeof(buf), stdin)) {
		if ((c = strchr(buf, '\n')))
			*c = '\0';
		if (!buf[0])
			break;	/* end of rules */
		if (nfields >= MAXFIELDS)	
			errx(1, "too many rules");
		rule = &rules[nfields++];
		rule->isdep = !!strstr(buf, "depart");
		if (sscanf(strchr(buf, ':'), ": %d-%d or %d-%d",
		    &rule->min[0], &rule->max[0],
		    &rule->min[1], &rule->max[1]) != 4)
			errx(1, "bad rule line: %s", buf);
	}

	while (1) {
		if (ntickets < 2)
			fgets(buf, sizeof(buf), stdin);
		if (ntickets == 1)
			fgets(buf, sizeof(buf), stdin);
		if (ntickets >= MAXTICKETS)
			errx(1, "too many tickets");
		ticket = &tickets[ntickets];
		for (i=0; i < nfields; i++)
			if (scanf("%d", &ticket->fs[i]) == 1)
				getchar();	/* , or \n */
			else if (i == 0)
				return;
			else
				errx(1, "%d: missing field", ntickets);
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
main(void)
{
	int ti,fi,ri, nassigned=0, nmatch, lastmatch=0;
	long part1=0, part2=1;

	parse();
	dump();

	for (ti=1; ti < ntickets; ti++)
	for (fi=0; fi < nfields; fi++) {
		for (ri=0; ri < nfields; ri++)
			if (isvalid(&tickets[ti], fi, &rules[ri]))
				goto anyvalid;
		tickets[ti].inval = 1;
		part1 += tickets[ti].fs[fi];
	anyvalid: ;
	}

	dump();
	printf("%ld\n", part1);

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

	dump();

	for (ri=0; ri < nfields; ri++)
		if (rules[ri].isdep)
			part2 *= tickets[0].fs[rules[ri].field];

	printf("%ld\n", part2);
}
