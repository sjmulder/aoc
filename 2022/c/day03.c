#include <stdio.h>
#include <string.h>
#include <limits.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

static inline int
value(unsigned char c)
{
	return c%32 + !(c >>5 &1)*26;
}

int
main()
{
	unsigned char line[64], seen[UCHAR_MAX+1], notseen[UCHAR_MAX+1];
	int p1=0, p2=0,  gpi=0;
	size_t len, i;

	memset(notseen, 0, sizeof(notseen));

	while (fgets((char *)line, sizeof(line), stdin)) {
		for (len=0; line[len] >= 32; len++)
			;
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

		for (i=0; i < LEN(seen); i++)
			notseen[i] = notseen[i] || !seen[i];

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
