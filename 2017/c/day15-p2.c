#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
main(void)
{
	uint64_t a = /*65*/ 591, b = /*8921*/ 393, n = 0, i;

	for (i = 0; i < 5000000; i++) {
		do { a *= 16807; a %= 0x7FFFFFFF; } while (a & 3);
		do { b *= 48271; b %= 0x7FFFFFFF; } while (b & 7);
		n += (a & 0xFFFF) == (b & 0xFFFF);
	}

	printf("%" PRIu64 "\n", n);
	return 0;
}
