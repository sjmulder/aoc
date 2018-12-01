#include <stdio.h>
#include <err.h>

int main()
{
	int accum = 0;
	int change;

	while (scanf(" %d", &change) == 1)
		accum += change;
	if (ferror(stdin))
		err(1, NULL);

	printf("%d\n", accum);
	return 0;
}
