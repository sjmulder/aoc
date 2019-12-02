#include <stdio.h>

int
main()
{
	int fuel = 0, mass;

	while (scanf(" %d", &mass) == 1)
		fuel += mass/3 - 2;

	printf("%d\n", fuel);
	return 0;
}
