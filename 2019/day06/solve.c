#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

typedef struct orb Orb;

struct orb {
	char name[4];
	Orb *par;
};

static int read_ln(char **, char **);
static Orb *get_orb(char *);
static size_t get_pars(Orb *, Orb **, size_t);

static Orb orbs[4096];
static size_t norbs;

int
main()
{
	static Orb *youpars[512];
	static Orb *sanpars[512];

	char *name, *pname;
	Orb *orb;
	size_t i, nyou, nsan;
	int ndeps = 0;

	while (read_ln(&pname, &name) == 1) {
		orb = get_orb(name);
		orb->par = get_orb(pname);
	}

	for (i = 0; i < norbs; i++) {
		for (orb = &orbs[i]; orb->par; orb = orb->par)
			ndeps++;
		if (strcmp(orb->name, "COM"))
			errx(1, "invalid root: %s", orb->name);
	}

	nyou = get_pars(get_orb("YOU"), youpars, LEN(youpars));
	nsan = get_pars(get_orb("SAN"), sanpars, LEN(sanpars));
	while (nyou && nsan && youpars[nyou-1] == sanpars[nsan-1]) {
		nyou--;
		nsan--;
	}

	printf("part 1: %d\n", ndeps);
	printf("part 2: %d\n", (int)(nyou + nsan));
	return 0;
}

static int
read_ln(char **pname, char **name)
{
	static char ln[16];
	char *c;

	if (!fgets(ln, LEN(ln), stdin))
		return 0;
	if (!(c = strchr(ln, ')')))
		errx(1, "no paren");
	*pname = ln; *c = '\0';
	*name = c+1;
	if ((c = strchr(*name, '\n')))
		*c = '\0';

	return 1;
}

static Orb *
get_orb(char *name)
{
	size_t i;
	int n;

	for (i = 0; i < norbs; i++)
		if (strcmp(orbs[i].name, name) == 0)
			return &orbs[i];

	if (++norbs > LEN(orbs))
		errx(1, "orb overflow");

	n = snprintf(orbs[i].name, LEN(orbs[i].name), "%s", name);
	if (n >= (int)LEN(orbs[i].name))
		errx(1, "name overflow");

	return &orbs[i];
}

static size_t
get_pars(Orb *orb, Orb **pars, size_t sz)
{
	size_t n = 0;

	for (; orb->par; orb = orb->par) {
		if (n >= sz)
			errx(1, "pars overflow");
		pars[n++] = orb->par;
	}
	if (strcmp(orb->name, "COM"))
		errx(1, "invalid root: %s", orb->name);

	return n;
}
