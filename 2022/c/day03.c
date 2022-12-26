#include "common.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

static int
value(unsigned char c)
{
	return c%32 + !(c >>5 &1)*26;
}

int
main(int argc, char **argv)
{
	unsigned char line[64];
	unsigned char seen[UCHAR_MAX+1];	/* chars seen for elf */
	unsigned char notseen[UCHAR_MAX+1];	/* not seen for group */
	int p1=0, p2=0,  gpi=0;
	size_t len, i;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	memset(notseen, 0, sizeof(notseen));

	while (fgets((char *)line, sizeof(line), stdin)) {
		for (len=0; line[len] >= 32; len++)
			;

		/* update seen[], check for dupes (p1) */
		memset(seen, 0, sizeof(seen));
		for (i=0; i < len/2; i++)
			seen[line[i]] = 1;
		for (; i < len; i++)
			if (!seen[line[i]])
				seen[line[i]] = 1;
			else {
				p1 += value(line[i]);
				break;
			}
		for (; i < len; i++)
			seen[line[i]] = 1;

		/* update notseen[] for the group */
		for (i=0; i < LEN(seen); i++)
			notseen[i] = notseen[i] || !seen[i];

		/* test and reset notseen[] at end of group (p2) */
		if (++gpi == 3) {
			for (i=0; i < LEN(notseen); i++)
				if (((i>='a' && i<='z') ||
				     (i>='A' && i<='Z')) &&
				    !notseen[i]) {
					p2 += value((unsigned char)i);
					break;
				}
			gpi = 0;
			memset(notseen, 0, sizeof(notseen));
		}
	}

	printf("03: %d %d\n", p1, p2);

	return 0;
}
