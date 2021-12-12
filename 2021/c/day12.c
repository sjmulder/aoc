#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define LEN(a)  (sizeof(a)/sizeof(*(a)))
#define BIG(id) (*(char*)&id < 'a')

#define START (*(uint16_t*)"start")
#define END   (*(uint16_t*)"end")

struct step;

struct link { uint16_t a, b; };
struct step { struct step *prev; uint16_t id; };

static struct link ls[32];
static int nls;

static void
dump_links(void)
{
	int i;

	for (i=0; i<nls; i++)
		printf("%.2s-%.2s\n", (char*)&ls[i].a,
		    (char*)&ls[i].b);
}

static void
dump_route(struct step *step)
{
	for (; step; step = step->prev)
		printf("%.2s,", (char*)&step->id);

	putchar('\n');
}

static int
search(struct step *prev, int maytwice)
{
	struct step *s;
	struct step next;
	int count=0, nvisits, i;

	next.prev = prev;

	//printf("searching ");
	//dump_route(prev);

	for (i=0; i<nls; i++) {
		if (ls[i].a == prev->id) next.id = ls[i].b; else
		if (ls[i].b == prev->id) next.id = ls[i].a; else
			continue;
		if (next.id == START) continue;
		if (next.id == END) {
			//printf("found ");
			//dump_route(&next);
			count++;
			continue;
		}

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
