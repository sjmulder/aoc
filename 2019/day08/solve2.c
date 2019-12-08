#include <stdio.h>
#include <limits.h>
#include <err.h>

#define L 100
#define W 25
#define H 6

int
main()
{
	static char layers[L][W*H];
	int l,i,r,c;

	if (fread(layers, 1, L*W*H, stdin) != L*H*W)
		errx(1, "input too small");

	for (l = 1; l < L; l++)
	for (i = 0; i < W*H; i++) {
		if (layers[0][i] == '2')
			layers[0][i] = layers[l][i];
	}

	for (r = 0; r < H; r++) {
		for (c = 0; c < W; c++)
			putchar(layers[0][r*W+c] == '1' ? '#' : ' ');
		putchar('\n');
	}
}
