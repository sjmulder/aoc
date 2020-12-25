#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../compat/string.h"

static int
validate(char *name, char *val)
{
	int ival, ilen;
	char *iend;

	ival = (int)strtol(val, &iend, 10);
	ilen = (int)(iend-val);

	if (!strcmp(name, "byr"))
		return ilen==4 && !*iend && ival>=1920 && ival<=2002;
	else if (!strcmp(name, "iyr"))
		return ilen==4 && !*iend && ival>=2010 && ival<=2020;
	else if (!strcmp(name, "eyr"))
		return ilen==4 && !*iend && ival>=2020 && ival<=2030;
	else if (!strcmp(name, "hgt")) {
		if (!strcmp(iend, "cm"))
			return ival>=150 && ival<=193;
		if (!strcmp(iend, "in"))
			return ival>=59 && ival<=76;
		else
			return 0;
	} else if (!strcmp(name, "hcl")) {
		if (*(val++) != '#')
			return 0;
		for (; *val; val++)
			if (!(*val>='0' && *val<='9') &&
			    !(*val>='a' && *val<='f')) return 0;
		return 1;
	} else if (!strcmp(name, "ecl")) {
		return !strcmp(val, "amb") || !strcmp(val, "blu") ||
		       !strcmp(val, "brn") || !strcmp(val, "gry") ||
		       !strcmp(val, "grn") || !strcmp(val, "hzl") ||
		       !strcmp(val, "oth");
	} else if (!strcmp(name, "pid"))
		return ilen==9 && !*iend;
	else
		return 0;
}

int
main(int argc, char **argv)
{
	static const char fields[][4] = {"byr", "iyr", "eyr", "hgt",
	    "hcl", "ecl", "pid"};

	FILE *f;
	char buf[256], *line, *rest, *lf, *name, *val;
	int i, p1=0,p2=0, nf=0,nok=0;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	do {
		line = rest = fgets(buf, sizeof(buf), f);
		if (!line || buf[0]=='\0' || buf[0]=='\n') {
			p1 += nf==7;
			p2 += nok==7;
			nok = nf = 0;
			continue;
		}

		if ((lf = strchr(line, '\n')))
			*lf = '\0';
		
		while ((val = strsep(&rest, " "))) {
			name = strsep(&val, ":");
			nok += validate(name, val);
			for (i=0; i<7; i++)
				nf += !!strstr(name, fields[i]);
		}
	} while (line);

	printf("%d %d\n", p1, p2);
	//getchar();
	return 0;
}
