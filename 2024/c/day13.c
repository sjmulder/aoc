#include "common.h"

static int64_t
score(int ax, int ay, int bx, int by, int64_t px, int64_t py)
{
	int64_t a,b, x;
	double as,bs;

	as = (double)ay / ax;
	bs = (double)by / bx;

	/* intersection between a (from start) and b (from end) */
	x = (int64_t)round((px*bs - py) / (bs-as));

	a = x / ax;
	b = (px-x) / bx;

	return
	    a*ax + b*bx == px &&
	    a*ay + b*by == py ? a*3 + b : 0;
}

int
main(int argc, char **argv)
{
	int ax,ay, bx,by;
	int64_t p1=0,p2=0, px,py;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (scanf(
	    " Button A: X+%d, Y+%d"
	    " Button B: X+%d, Y+%d"
	    " Prize: X=%"SCNd64", Y=%"SCNd64,
	    &ax, &ay, &bx, &by, &px, &py) == 6) {
		p1 += score(ax,ay, bx,by, px,py);
		p2 += score(ax,ay, bx,by,
		    px + 10000000000000,
		    py + 10000000000000);
	}

	printf("13: %"PRId64" %"PRId64"\n", p1, p2);
	return 0;
}
