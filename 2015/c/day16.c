#include <stdio.h>
#include <string.h>
#include <assert.h>

#define NAME_IS(x)	(!strcmp(name, x))

int
main()
{
	char line[128], name[16];
	char *end, *attr, *rest;
	int p1=0, p2=0, sue=0, match_p1, match_p2, count, n;

	while (fgets(line, sizeof(line), stdin)) {
		sue++;

		if ((end = strchr(line, '\n')))
			*end = '\0';

		/* skip 'Sue %d: ' */
		rest = strchr(line, ':');
		assert(rest);
		assert(rest[1] == ' ');
		rest += 2;

		match_p1 = match_p2 = 1;

		while ((attr = strsep(&rest, ","))) {
			n = sscanf(attr, " %15s %d", name, &count);
			assert(n == 2);
			if ((end = strchr(name, ':')))
				*end = '\0';

			match_p1 = match_p1 && !(
			    (NAME_IS("children")    && count != 3) ||
			    (NAME_IS("cats")        && count != 7) ||
			    (NAME_IS("samoyeds")    && count != 2) ||
			    (NAME_IS("pomeranians") && count != 3) ||
			    (NAME_IS("akitas")      && count != 0) ||
			    (NAME_IS("vizslas")     && count != 0) ||
			    (NAME_IS("goldfish")    && count != 5) ||
			    (NAME_IS("trees")       && count != 3) ||
			    (NAME_IS("cars")        && count != 2) ||
			    (NAME_IS("perfumes")    && count != 1));

			match_p2 = match_p2 && !(
			    (NAME_IS("children")    && count != 3) ||
			    (NAME_IS("cats")        && count  < 7) ||
			    (NAME_IS("samoyeds")    && count != 2) ||
			    (NAME_IS("pomeranians") && count  > 3) ||
			    (NAME_IS("akitas")      && count != 0) ||
			    (NAME_IS("vizslas")     && count != 0) ||
			    (NAME_IS("goldfish")    && count  > 5) ||
			    (NAME_IS("trees")       && count  < 3) ||
			    (NAME_IS("cars")        && count != 2) ||
			    (NAME_IS("perfumes")    && count != 1));
		}

		if (match_p1) p1 = sue;
		if (match_p2) p2 = sue;
	}


	printf("16: %d %d\n", p1, p2);
	return 0;
}
