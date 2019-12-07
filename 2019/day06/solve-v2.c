#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

#define MAXORB (256*1024)

#define COM_ID (((int)'C'<<16) | ((int)'O'<<8) | 'M')
#define YOU_ID (((int)'Y'<<16) | ((int)'O'<<8) | 'U')
#define SAN_ID (((int)'S'<<16) | ((int)'A'<<8) | 'N')

typedef struct orb Orb;

struct orb {
	int id;
	Orb *par;	/* parent, that is, the orbitee */
};

static int read_ln(int *, int *);
static Orb *get_orb(int);
static size_t get_pars(Orb *, Orb **, size_t);

static Orb orbs[MAXORB];
static size_t norbs;

int
main()
{
	static Orb *youpars[MAXORB];
	static Orb *sanpars[MAXORB];

	int id, parid;
	Orb *orb;
	size_t i, nyou, nsan;
	int ndeps = 0;

	fprintf(stderr, " reading input");
	while (read_ln(&parid, &id) == 1) {
		if (!(norbs & 0x3FF)) /* every 1024 */
			fputc('.', stderr);
		orb = get_orb(id);
		orb->par = get_orb(parid);
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
		if (orb->id != COM_ID)
			errx(1, "invalid root");
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
	nyou = get_pars(get_orb(YOU_ID), youpars, LEN(youpars));
	nsan = get_pars(get_orb(SAN_ID), sanpars, LEN(sanpars));
	while (nyou && nsan && youpars[nyou-1] == sanpars[nsan-1]) {
		nyou--;
		nsan--;
	}

	printf("part 1: %d\n", ndeps);
	printf("part 2: %d\n", (int)(nyou + nsan));
	return 0;
}

static int
read_ln(int *parid, int *id)
{
	static char ln[16];
	char *c;

	if (!fgets(ln, LEN(ln), stdin))
		return 0;

	c = ln; *parid = 0; *id = 0;
	while (*c != ')')
		*parid = (*parid << 8) | *(c++);
	c++;
	while (*c && *c != '\n')
		*id = (*id << 8) | *(c++);

	return 1;
}

/*
 * Get or create an orbit with the given id.
 */
static Orb *
get_orb(int id)
{
	size_t i;

	for (i = 0; i < norbs; i++)
		if (orbs[i].id == id)
			return &orbs[i];

	if (++norbs > LEN(orbs))
		errx(1, "orb overflow");
	orbs[i].id = id;

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
	if (orb->id != COM_ID)
		errx(1, "invalid root");

	return n;
}
