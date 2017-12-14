#define _CRT_SECURE_NO_WARNINGS
#define NROUNDS 64

#include <stdio.h>
#include <string.h>
#include "khash.h"

static const char salt[] = {17, 31, 73, 47, 23};

static void
flood(int color, int x, int y, int colors[128][128])
{
	colors[y][x] = color;

	if (x     && colors[y][x-1] == -1) flood(color, x-1, y, colors);
	if (x<127 && colors[y][x+1] == -1) flood(color, x+1, y, colors);
	if (y     && colors[y-1][x] == -1) flood(color, x, y-1, colors);
	if (y<127 && colors[y+1][x] == -1) flood(color, x, y+1, colors);
}

int
main(int argc, char **argv)
{
	int y, x, i;
	int colors[128][128], ncolors = 0;
	char suffix[5], hash[KHASH_SZ];
	struct khash_state state;

	if (argc < 2) {
		fputs("usage: day14a KEY\n", stderr);
		return 1;
	}

	memset(colors, 0, sizeof(colors));

	for (y = 0; y < 128; y++) {
		sprintf(suffix, "-%d", y);

		khash_begin(&state);
		for (i = 0; i < NROUNDS; i++) {
			khash_append(&state, argv[1], -1);
			khash_append(&state, suffix, -1);
			khash_append(&state, salt, sizeof(salt));
		}
		khash_build(&state, hash);

		for (x = 0; x < 128; x++)
			colors[y][x] = -((hash[x/8] >> (7-x%8)) & 1);
	}

	for (y = 0; y < 128; y++)
		for (x = 0; x < 128; x++)
			if (colors[y][x] == -1)
				flood(++ncolors, x, y, colors);

	for (y = 0; y < 128; y++) {
		for (x = 0; x < 128; x++) {
			if (colors[y][x])
				putchar('a' + (colors[y][x]-1) % 26);
			else
				putchar(' ');
		}
		putchar('\n');
	}

	printf("%d\n", ncolors);
	return 0;
}
