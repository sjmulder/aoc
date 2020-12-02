#include <stdio.h>

int
main()
{
	int i;

	for (i=0; i<99998; i++)
		puts("100");

	puts("900");
	puts("1100");

	for (i=0; i<99998; i++)
		puts("1900");

	puts("20");
	puts("1120");
}
