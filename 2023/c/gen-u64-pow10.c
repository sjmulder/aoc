#include <stdio.h>
#include <stdint.h>

int
main()
{
	int i;
	uint64_t n=1;

	for (i=0; i<20; i++) {
		printf("    {0x%08lx, 0x%08lx},\n",
		    n & 0xFFFFFFFFU,
		    (n >> 32) & 0xFFFFFFFFU);
		n *= 10;
	}
}
