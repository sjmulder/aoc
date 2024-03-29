#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

#define MAXORB (256*1024)

typedef struct orb Orb;

struct orb {
	char name[4];
	Orb *par;	/* parent, that is, the orbitee */
};

static int read_ln(char **, char **);
static Orb *get_orb(char *);
static size_t get_pars(Orb *, Orb **, size_t);

static Orb orbs[MAXORB];
static size_t norbs;

int
main()
{
	static Orb *youpars[MAXORB];
	static Orb *sanpars[MAXORB];

	char *name, *pname;
	Orb *orb;
	size_t i, nyou, nsan;
	int ndeps = 0;

	fprintf(stderr, " reading input");
	while (read_ln(&pname, &name) == 1) {
		if (!(norbs & 0x3FF)) /* every 1024 */
			fputc('.', stderr);
		orb = get_orb(name);
		orb->par = get_orb(pname);
	}
	fputc('\n', stderr);

	/*
	 * Calculate the number of direct and indirect orbits by
	 * iterating over al orbits, then coutning the number of steps
	 * to the root.
	 */
	fprintf(stderr, " calculating deps");
	for (i = 0; i < norbs; i++) {
		if (!(i & 0x3FF)) /* every 1024 */
			fputc('.', stderr);
		for (orb = &orbs[i]; orb->par; orb = orb->par)
			ndeps++;
		if (strcmp(orb->name, "COM"))
			errx(1, "invalid root: %s", orb->name);
	}
	fputc('\n', stderr);

	/*
	 * Calculate the number of steps between YOU and SAN(ta). We
	 * know both eventually have COM as the final ancestor. The
	 * shortest path between them is found when we remove the
	 * shared ancestors:
	 *
	 *      YOU->a->b-> f->g->COM
	 *   SAN->c->d->e-> f->g->COM
	 *                 ^ cut here
	 */
	fprintf(stderr, " calculating path length\n");
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

	/* can't use scanf because there's no whitespace */

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

/*
 * Get or create an orbit with the given name.
 */
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

/*
 * Get the full list of all ancestors (parents, 'pars') of the given 
 * orbit, starting at orb and ending with COM.
 */
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
