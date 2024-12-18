#include "common.h"

#define GZ 52

static char g[GZ][GZ];
#define ANTI_P1 1
#define ANTI_P2 2
static uint8_t anti[GZ][GZ];
static int w,h;

int
main(int argc, char **argv)
{
	int p1=0,p2=0, x,y, x1,y1, ax,ay, i;
	char *lf;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	for (h=0; h<GZ && fgets(g[h], GZ, stdin); h++)
		;

	assert(feof(stdin));
	lf = strchr(g[0], '\n');
	assert(lf);
	w = (int)(lf - g[0]);

	/*
	 * Find antenna pairs, then project backwards from the first,
	 * forwards from the second. Don't like the repetition but it
	 * makes for easy code.
	 */
	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		if (!isalnum(g[y][x]))
			continue;

		for (y1=y; y1<h; y1++)
		for (x1=(y==y1?x+1:0); x1<w; x1++) {
			if (g[y][x] != g[y1][x1])
				continue;

			for (i=0; ; i++) {
				if ((ax = x-(x1-x)*i) <0 || ax>w ||
				    (ay = y-(y1-y)*i) <0 || ay>h)
					break;
				anti[ay][ax] |= ANTI_P1 * i==1;
				anti[ay][ax] |= ANTI_P2;
			}

			for (i=0; ; i++) {
				if ((ax = x1+(x1-x)*i) <0 || ax>w ||
				    (ay = y1+(y1-y)*i) <0 || ay>h)
					break;
				anti[ay][ax] |= ANTI_P1 * i==1;
				anti[ay][ax] |= ANTI_P2;
			}
		}
	}

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		p1 += !!(anti[y][x] & ANTI_P1);
		p2 += !!(anti[y][x] & ANTI_P2);
	}

	printf("08: %d %d\n", p1, p2);
	return 0;
}
