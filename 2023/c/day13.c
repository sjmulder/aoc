#include "common.h"

int
main(int argc, char **argv)
{
	static char g[32][32];
	int p1=0,p2=0, w,h, x,y,i, nmis;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (!feof(stdin)) {
		for (h=0; ; h++) {
			assert(h < (int)LEN(*g));
			if (!fgets(g[h], LEN(*g), stdin)) break;
			if (!g[h][0] || g[h][0]=='\n') break;
		}

		assert(h>0); w = (int)strlen(g[0])-1;
		assert(w>0);

		for (x=1; x<w; x++) {
			nmis = 0;
			for (i=0; i<x && x+i<w; i++)
			for (y=0; y<h; y++)
				nmis += g[y][x-i-1] != g[y][x+i];
			if (nmis==0) p1 += x; else
			if (nmis==1) p2 += x;
		}

		for (y=1; y<h; y++) {
			nmis = 0;
			for (i=0; i<y && y+i<h; i++)
			for (x=0; x<w; x++)
				nmis += g[y-i-1][x] != g[y+i][x];
			if (nmis==0) p1 += y*100; else
			if (nmis==1) p2 += y*100;
		}
	}

	printf("13: %d %d\n", p1, p2);
	return 0;
}
