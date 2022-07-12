#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

static char map_from[64][4];
static char map_to[64][16];
static size_t nmap;

static char input[512];
static char perms[1024][512];
static size_t nperm;

int
main()
{
	static char buf[1024];
	char *lf, *from, *match;
	size_t i,j;
	int p1=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "%3s => %15s",
		    map_from[nmap], map_to[nmap]) != 2)
			break;
		if (nmap++ >= LEN(map_from))
			errx(1, "map overflow");
	}

	if (!fgets(input, sizeof(input), stdin))
		errx(1, "unexpected EOF reading input string");
	if ((lf = strchr(input, '\n')))
		*lf = '\0';

	for (i=0; i<nmap; i++) {
		from = input;
		while ((match = strstr(from, map_from[i]))) {
			snprintf(perms[nperm], sizeof(*perms),
			    "%.*s%s%s", (int)(match-input), input,
			    map_to[i], match+strlen(map_from[i]));
			if (++nperm >= LEN(perms))
				errx(1, "strs overflow");
			from = match+1;
		}
	}

	for (i=0; i<nperm; i++) {
		for (j=0; j<i; j++)
			if (!strcmp(perms[i], perms[j]))
				goto not_uniq;
		p1++;
	not_uniq: ;
	}

	printf("19: %d\n", p1);
	return 0;
}
