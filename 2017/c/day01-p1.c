#include <stdio.h>
#include <ctype.h>

int
main(void)
{
	int c, fst, prev = EOF, sum = 0;

	fst = c = getchar();
	while (c != EOF) {
		if (isdigit(c)) {
			if (prev == c)
				sum += c-'0';
			prev = c;
		}
		c = getchar();
	}

	if (fst != EOF && prev == fst)
		sum += fst-'0';

	printf("%d\n", sum);
	return 0;
}
