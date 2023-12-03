#include "common.h"

#define GSZ	160

static char G[GSZ][GSZ];
static int N[GSZ][GSZ];

int
main(int argc, char **argv)
{

	int p1=0,p2=0, h=0, x,y, dx,dy, n=0,sym=0,r;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (h=0; fgets(&G[h+1][1], GSZ-1, stdin); h++)
		assert(h < GSZ);

	/*
	 * Pass 1: parse numbers and solve part 1. For every digit in
	 * G, the full number it is part of is stored in N.
	 */
	for (y=1; y<=h; y++)
	for (x=1; G[y][x]; x++)
		if (isdigit(G[y][x])) {
			n = n*10 + G[y][x]-'0';

			for (dy=-1; dy<2; dy++)
			for (dx=-1; dx<2; dx++)
				sym = sym || (x && y &&
				    G[y+dy][x+dx] != '.' &&
				    ispunct(G[y+dy][x+dx]));
		} else {
			for (dx=-1; isdigit(G[y][x+dx]); dx--)
				N[y][x+dx] = n;
			if (sym)
				p1 += n;
			n = sym = 0;
		}

	/*
	 * Pass 2: solve part 2 by finding all '*', then counting and
	 * multiplying adjecent numbers.
	 *
	 * Horizontal adjecency is trivial but vertical/diagonal has
	 * two situations: if there's a digit directly North of the '+',
	 * it must be a single number: NW and NE would connect to it.
	 * If N isn't a digit, digits in NW and NE belong to separate
	 * numbers.
	 */
	for (y=1; y<=h; y++)
	for (x=1; G[y][x]; x++) {
		if (G[y][x] != '*')
			continue;

		n = 0; r = 1;

		if (N[y][x-1]) { n++; r += N[y][x-1]; }
		if (N[y][x+1]) { n++; r += N[y][x+1]; }

		if (N[y-1][x]) { n++; r += N[y-1][x]; } else {
			if (N[y-1][x-1]) { n++; r += N[y-1][x-1]; }
			if (N[y-1][x+1]) { n++; r += N[y-1][x+1]; }
		}

		if (N[y+1][x]) { n++; r += N[y+1][x]; } else {
			if (N[y+1][x-1]) { n++; r += N[y+1][x-1]; }
			if (N[y+1][x+1]) { n++; r += N[y+1][x+1]; }
		}

		if (n == 2)
			p2 += r;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
