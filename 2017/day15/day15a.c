#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
main(void)
{
	uint64_t a = 591, b = 393, n = 0, i;

	for (i = 0; i < 40000000; i++) {
		a *= 16807; a %= 0x7FFFFFFF;
		b *= 48271; b %= 0x7FFFFFFF;
		n += (a & 0xFFFF) == (b & 0xFFFF);
	}

	printf("%" PRIu64 "\n", n);
	return 0;
}
