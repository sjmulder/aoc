#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/*
 * Implemented as a depth-first search using recursion. The route is
 * kept in a linked list pointing *back* to the visited notes. This
 * makes managing the list trivial - each level of recursion just keeps
 * its own link on the stack.
 *
 * To avoid a bunch of strcmp(), nodes are identified by interpreting
 * the first two bytes of their name strings as 16-bit number.
 */

#define LEN(a)	(sizeof(a)/sizeof(*(a)))
#define BIG(id)	(*(char*)&id < 'a')

#define START	(*(uint16_t*)"start")
#define END	(*(uint16_t*)"end")

struct step;
struct link { uint16_t a, b; };
struct step { struct step *prev; uint16_t id; };

static struct link ls[32];
static int nls;

static int
search(struct step *prev, int maytwice)
{
	struct step next, *s;
	int count=0, nvisits, i;

	next.prev = prev;

	for (i=0; i<nls; i++) {
		if (ls[i].a == prev->id) next.id = ls[i].b; else
		if (ls[i].b == prev->id) next.id = ls[i].a; else
			continue;
		if (next.id == START) continue;
		if (next.id == END) { count++; continue; }

		if (BIG(next.id))
			count += search(&next, maytwice);
		else {
			for (nvisits=0, s=prev; s; s=s->prev)
				nvisits += s->id == next.id;
			if (nvisits > maytwice) continue;
			count += search(&next, maytwice && !nvisits);
		}
	}

	return count;
}

int
main()
{
	char buf[16], *rest,*lf,*part;
	int p1,p2;
	struct step step;

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		if ((lf = strchr(buf, '\n'))) *lf = '\0';
		part = strsep(&rest, "-");
		assert(part); assert(strlen(part));
		assert(rest); assert(strlen(rest));
		ls[nls].a = *(uint16_t*)part;
		ls[nls].b = *(uint16_t*)rest;
		nls++;
		assert(nls < (int)LEN(ls));
	}

	step.prev = NULL;
	step.id = START;
	p1 = search(&step, 0);
	p2 = search(&step, 1);

	printf("12: %d %d\n", p1, p2);
	return 0;
}
