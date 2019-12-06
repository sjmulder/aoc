#include <stdio.h>

static const char chars[] =
"abcdefghijklmnopqrstuvwxyz"
/* "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
"0123456789";

int
main()
{
	size_t i, j, k;

	printf("COM)");

	for (i = 0; i < sizeof(chars)-1; i++)
	for (j = 0; j < sizeof(chars)-1; j++)
	for (k = 0; k < sizeof(chars)-1; k++)
		printf("%c%c%c\n%c%c%c)",
		    chars[i], chars[j], chars[k],
		    chars[i], chars[j], chars[k]);
	
	printf("SAN\nSAN)YOU\n");
	return 0;
}
