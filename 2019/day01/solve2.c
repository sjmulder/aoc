#include <stdio.h>

int
main()
{
	int total = 0, fuel;

	while (scanf(" %d", &fuel) == 1)
		while ((fuel = fuel/3 - 2) > 0)
			total += fuel;

	printf("%d\n", total);
	return 0;
}
