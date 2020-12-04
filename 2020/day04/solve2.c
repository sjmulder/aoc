#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
main()
{
	char buf[256], *line, *rest, *lf, *name, *val;
	int nok_total=0, nok=0;

	do {
		line = rest = fgets(buf, sizeof(buf), stdin);
		if (!line || buf[0]=='\0' || buf[0]=='\n')
			{nok_total += nok==7; nok = 0; continue;}
		if ((lf = strchr(line, '\n')))
			*lf = '\0';
		while ((val = strsep(&rest, " "))) {
			name = strsep(&val, ":");
			nok += validate(name, val);
		}
	} while (line);

	printf("%d\n", nok_total);
}
