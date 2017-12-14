#include <string.h>
#include "khash.h"

#define BLOCKSZ  16
#define ARRSZ   256

void
khash_begin(struct khash_state *state)
{
	int i;

	state->pos = 0;
	state->skip = 0;

	for (i = 0; i < ARRSZ; i++)
		state->arr[i] = i;
}

void
khash_append(struct khash_state *state, const char *data, int n)
{
	int i, j;
	char *p, *q, temp;
	unsigned char length;

	for (i = 0; n == -1 ? data[i] : i < n; i++) {
		length = (unsigned char)data[i];

		for (j=0; j < length / 2; j++) {
			p = state->arr + (state->pos + j) % ARRSZ;
			q = state->arr + (state->pos + length-j-1) % ARRSZ;
			temp = *p; *p = *q; *q = temp;
		}

		state->pos += length + state->skip++;
	}
}

void
khash_build(struct khash_state *state, char *dest)
{
	int block, i;

	memset(dest, 0, KHASH_SZ);
	for (block = 0; block < KHASH_SZ; block++)
		for (i = 0; i < BLOCKSZ; i++)
			dest[block] ^= state->arr[block*BLOCKSZ+i];
}
