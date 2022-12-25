#include <stdio.h>

int
main()
{
	int i,c, p1=0, p2=0;

	for (i=1; (c = getchar()) != EOF; i++)
		if ((p1 += c=='(' ? 1 : c==')' ? -1 : 0) == -1 && !p2)
			p2=i;

	printf("01: %d %d\n", p1, p2);
	return 0;
}
