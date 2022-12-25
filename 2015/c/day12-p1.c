#include <stdio.h>
#include <ctype.h>

int
main()
{
	int c=0, n=0, sign=1, acc=0;

	while (c != EOF)
		if (isdigit((c = getchar())))
			n = n*10 + (c-'0');
		else {
			acc += sign * n;
			sign = c=='-' ? -1 : 1;
			n = 0;
		}

	printf("%d\n", acc);
	return 0;
}
