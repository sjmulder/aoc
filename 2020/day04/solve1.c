#include <stdio.h>
#include <string.h>

int main() {
	static const char fields[][5] = {"byr:", "iyr:", "eyr:", "hgt:",
	    "hcl:", "ecl:", "pid:"};
	char buf[256], *p;
	int nok=0, nf=0, i;

	do {
		p = fgets(buf, sizeof(buf), stdin);
		if (!p || buf[0]=='\0' || buf[0]=='\n')
			{nok += nf == 7; nf = 0; continue;}
		for (i=0; i<7; i++)
			nf += !!strstr(buf, fields[i]);
	} while (p);

	printf("%d\n", nok);
}
