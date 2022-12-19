#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))
#define NRES	4

struct bp { int price[NRES][NRES]; };
struct st { struct blueprint *bp; int t, res[NRES], robos[NRES]; };

struct bp bps[32];
size_t nbp;

static void
read_input(void)
{
	static char buf[196];
	char *rest, *s;
	int rres, cres, cost;

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		assert(nbp < LEN(bps));
		nbp++;

		rest = strchr(rest, ':'); assert(rest);
		rest++; assert(rest);

		while ((s = strsep(&rest, "."))) {
			if (!*s || *s == '\n' || *s == '\r')
				break;

			rres =
			    !strncmp(s, " Each or", 8) ? 0 :
			    !strncmp(s, " Each cl", 8) ? 1 :
			    !strncmp(s, " Each ob", 8) ? 2 :
			    !strncmp(s, " Each ge", 8) ? 3 : -1;
			assert(rres != -1);
			assert(rres < NRES);

			while (1) {
				while (*s && !isdigit(*s))
					s++;
				if (!*s)
					break;

				cost = (int)strtol(s, &s, 10);
				assert(cost >= 0);

				cres =
				    !strncmp(s, " or", 3) ? 0 :
				    !strncmp(s, " cl", 3) ? 1 :
				    !strncmp(s, " ob", 3) ? 2 :
				    !strncmp(s, " ge", 3) ? 3 : -1;
				assert(cres != -1);
				assert(cres < NRES);

				bps[nbp-1].price[rres][cres] = cost;
				s += 4;
			}
		}
	}
}

static void
print_blueprints(void)
{
	size_t i,j;

	for (i=0; i<nbp; i++) {
		printf("Blueprint %zu:\n", i);

		for (j=0; j<NRES; j++)
			printf(" robot %zu costs %d/%d/%d/%d\n", j,
			    bps[i].price[j][0],
			    bps[i].price[j][1],
			    bps[i].price[j][2],
			    bps[i].price[j][3]);
	}
}

int
main()
{
	read_input();
	print_blueprints();

	return 0;
}
