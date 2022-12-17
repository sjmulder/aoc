#define _CRT_SECURE_NO_WARNINGS
#define NROUNDS 64
#define PRINT    1

#include <stdio.h>
#include "khash.h"

static const char salt[] = {17, 31, 73, 47, 23};

int
main(int argc, char **argv)
{
	int i, j, k, nones, nones_total = 0;
	char suffix[5], hash[KHASH_SZ];
	struct khash_state state;

	if (argc < 2) {
		fputs("usage: day14a KEY\n", stderr);
		return 1;
	}

	for (i = 0; i < 128; i++) {
		sprintf(suffix, "-%d", i);

		khash_begin(&state);	
		for (j = 0; j < NROUNDS; j++) {
			khash_append(&state, argv[1], -1);
			khash_append(&state, suffix, -1);
			khash_append(&state, salt, sizeof(salt));
		}
		khash_build(&state, hash);

		nones = 0;
		for (j = 0; j < KHASH_SZ; j++)
			for (k = 0; k < 8; k++)
				nones += (hash[j] >> k) & 1;
		
		nones_total += nones;

#ifdef PRINT
		printf("%s%-4s ", argv[1], suffix);
		for (j = 0; j < KHASH_SZ; j++)
			printf("%02x", (unsigned char)hash[j]);
		putchar(' ');
		for (j = 0; j < KHASH_SZ; j++)
			for (k = 7; k >= 0; k--)
				putchar((hash[j] >> k) & 1 ? '#' : '.');
		printf(" +%2d = %4d\n", nones, nones_total);
#endif
	}

	printf("%d\n", nones_total);
	return 0;
}
