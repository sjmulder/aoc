#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>

int main()
{
	int accum = 0;
	int change;
	unsigned byte, bit;
	char *bitfield;

	if (!(bitfield = calloc(UINT_MAX / 8, 1)))
		err(1, "calloc");

	bitfield[0] = 1;

	while (scanf(" %d", &change) == 1) {
		accum += change;

		byte = (unsigned)accum / 8;
		bit = 1 << ((unsigned)accum % 8);

		if (bitfield[byte] & bit) {
			printf("%d\n", accum);
			return 0;
		}

		bitfield[byte] = bitfield[byte] | bit;
	}

	if (ferror(stdin))
		err(1, NULL);

	puts("no duplicates");
	return 0;
}
