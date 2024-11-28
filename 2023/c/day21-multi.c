#include "common.h"

#define MULTI 5
#define GSZ (133*MULTI)

/*
 * Simple grid-based implementation except the grid is repeated MULTI
 * times. Used to test and compare the 'mathematical' solution used for
 * part 2. Also prints a pretty grid.
 */
int
main(int argc, char **argv)
{
	static char map[GSZ][GSZ];
	static uint8_t reach[GSZ][GSZ][2];	/* time dim. %2 */
	int len, sz, x,y,t, i,j;
	int nreach_all=0, nreach_1map=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=0; fgets(map[y], sizeof(map[y]), stdin); y++) {
		len = (int)strlen(map[y]);
		assert(len);
		assert(map[y][len-1] == '\n');
		assert(y+1 < GSZ/MULTI);
	}

	sz = y;
	assert(sz > 0);
	assert(map[0][sz] == '\n');

	for (i=1; i<MULTI; i++)
	for (y=0; y<sz; y++)
	for (x=0; x<sz; x++)
		map[y][i*sz+x] = map[y][x];

	for (i=1; i<MULTI; i++)
	for (y=0; y<sz; y++)
	for (x=0; x<sz*MULTI; x++)
		map[i*sz+y][x] = map[y][x];

	for (y=MULTI/2*sz; y<(MULTI/2+1)*sz; y++)
	for (x=MULTI/2*sz; x<(MULTI/2+1)*sz; x++)
		if (map[y][x] == 'S') {
			reach[y][x][0] = 1;
			break;
		}

	for (t=0; t<(131*2+65); t++)
	for (y=0; y<sz*MULTI; y++)
	for (x=0; x<sz*MULTI; x++)
		reach[y][x][(t+1)&1] =
		    (map[y][x] == '.' || map[y][x] == 'S') &&
		    ((x>0     && reach[y][x-1][t&1]) ||
		     (x<GSZ-1 && reach[y][x+1][t&1]) ||
		     (y>0     && reach[y-1][x][t&1]) ||
		     (y<GSZ-1 && reach[y+1][x][t&1]));
	
#if 1
	for (y=0; y<sz*MULTI; y++) {
		if (y && !(y % sz)) {
			for (x=0; x<sz*MULTI; x++) {
				if (x && !(x % sz))
					putchar('+');
				putchar('-');
			}
			putchar('\n');
		}
		for (x=0; x<sz*MULTI; x++) {
			if (x && !(x % sz))
				putchar('|');

			if (reach[y][x][0]) printf("\033[31mo"); else
			if (reach[y][x][1]) printf("\033[32mx"); else
				putchar(map[y][x]);

			printf("\033[0m");
		}
		putchar('\n');
	}
#endif
		
	for (y=0; y<sz*MULTI; y++)
	for (x=0; x<sz*MULTI; x++)
		nreach_all += reach[y][x][t&1];

	printf("reachable: %d\n", nreach_all);
	printf("per map:\n");

	for (i=0; i<MULTI; i++) {
		for (j=0; j<MULTI; j++) {
			nreach_1map = 0;

			for (y=i*sz; y<(i+1)*sz; y++)
			for (x=j*sz; x<(j+1)*sz; x++)
				nreach_1map += reach[y][x][t&1];

			printf(" %6d", nreach_1map);
		}

		putchar('\n');
	}

	return 0;
}
