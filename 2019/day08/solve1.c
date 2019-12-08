#include <stdio.h>
#include <limits.h>
#include <err.h>

#define L 100
#define W 25
#define H 6

static int count(int, char);

char layers[L][W*H];

int
main()
{
	int l, num0, best = INT_MAX, bestl = 0;

	if (fread(layers, 1, L*W*H, stdin) != L*H*W)
		errx(1, "input too small");

	for (l = 0; l < L; l++)
		if ((num0 = count(l, '0')) < best) {
			best = num0;
			bestl = l;
		}

	printf("%d\n", count(bestl, '1') * count(bestl, '2'));
}

static int
count(int l, char val)
{
	int i, acc = 0;

	for (i = 0; i < W*H; i++)
		acc += layers[l][i] == val;

	return acc;
}
