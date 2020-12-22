#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

#define NAMESZ 16

/* ingredient or allergen */
struct thing { char nm[16]; struct thing *other; };

struct meal {
	int ingreds[128];	/* indices into global ingreds[] */
	int allergs[8];		/* indices into global allergs[] */
	int ningreds, nallergs;
};

static struct meal meals[64];
static struct thing ingreds[256];
static struct thing allergs[32];
static int nmeals, ningreds, nallergs;

/* ingredient<->allergent possibility matrix */
static char canbe[LEN(ingreds)][LEN(allergs)];

static int
getthing(struct thing *things, int *num, char *nm)
{
	int i;

	for (i=0; i < *num; i++)
		if (!strcmp(nm, things[i].nm))
			return i;

	(*num)++;
	snprintf(things[i].nm, sizeof(things[i].nm), "%s", nm);

	return i;
}

static void
parse(void)
{
	struct meal *meal=NULL;
	char word[16], *cp;
	int newmeal=1, inallergs=0;

	assert(sizeof(word) == sizeof(ingreds[0].nm));
	assert(sizeof(word) == sizeof(allergs[0].nm));

	while (scanf("%s\n", word) == 1) {
		if (newmeal) {
			assert(nmeals < LEN(meals));
			meal = &meals[nmeals++];
			newmeal = inallergs = 0;
		}

		if (word[0] == '(')
			inallergs = 1;
		else if (inallergs) {
			if ((cp = strchr(word, ',')))
				*cp = '\0';
			else if ((cp = strchr(word, ')')))
				{ *cp = '\0'; newmeal = 1; }

			assert(nallergs < LEN(allergs));
			assert(meal->nallergs < LEN(meal->allergs));

			meal->allergs[meal->nallergs++] =
			    getthing(allergs, &nallergs, word);
		} else {
			assert(ningreds < LEN(ingreds));
			assert(meal->ningreds < LEN(meal->ingreds));

			meal->ingreds[meal->ningreds++] =
			    getthing(ingreds, &ningreds, word);
		}
	}
}

static void
pair(int ingred, int allerg)
{
	int i;

	ingreds[ingred].other = &allergs[allerg];
	allergs[allerg].other = &ingreds[ingred];

	for (i=0; i<ningreds; i++)
		canbe[i][allerg] = i == ingred;
	for (i=0; i<nallergs; i++)
		canbe[ingred][i] = i == allerg;
}

/*
 * Initialise canbe but cross out any combination in which the
 * ingredient does't appear in all meals where the allergent is listed.
 */
static void
solve_prep(void)
{
	int ingred, allerg, m,i;

	for (ingred=0; ingred<ningreds; ingred++)
	for (allerg=0; allerg<nallergs; allerg++) {
		for (m=0; m<nmeals; m++) {
			for (i=0; i<meals[m].nallergs; i++)
				if (meals[m].allergs[i] == allerg)
					break;
			if (i == meals[m].nallergs)
				continue;  /* allergent not in meal */

			for (i=0; i<meals[m].ningreds; i++)
				if (meals[m].ingreds[i] == ingred)
					break;
			if (i == meals[m].ningreds)
				break;  /* ingredient not in meal */
		}

		/* iff ingredient in all meals with allergen */
		canbe[ingred][allerg] = m == nmeals;
	}
}

/*
 * Find all allergens only have one ingredient set in canbe and pair
 * them. Not the other way around because not every ingredient needs to
 * have an allergen.
 */
static int
solve_crisscross(void)
{
	int i,j, count,last, nchanges=0;

	for (i=0; i<nallergs; i++) {
		count = last = 0;
		if (allergs[i].other)
			continue;
		for (j=0; j<ningreds; j++)
			if (canbe[j][i])
				{ last=j; count++; }
		if (count == 1)
			{ pair(last, i); nchanges++; }
	}

	return nchanges;
}

static int
cmp_nm(const void *a, const void *b)
{
	const struct thing *ta = a;
	const struct thing *tb = b;

	return strcmp(ta->nm, tb->nm);
}

int
main()
{
	int i,j, p1=0;

	parse();
	solve_prep();

	while (solve_crisscross())
		;

	for (i=0; i<nallergs; i++)
		if (!allergs[i].other) {
			printf("\nunresolved: %s\n", allergs[i].nm);
			return 1;
		}

	for (i=0; i<nmeals; i++)
	for (j=0; j<meals[i].ningreds; j++)
		if (!ingreds[meals[i].ingreds[j]].other)
			p1++;

	printf("%d\n", p1);

	/* note, breaks references to allergs[] */
	qsort(allergs, nallergs, sizeof(allergs[0]), cmp_nm);

	for (i=0; i<nallergs; i++)
		printf("%s%s", i?",":"", allergs[i].other->nm);
	putchar('\n');
}
