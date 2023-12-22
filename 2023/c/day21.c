#include "common.h"

#define GSZ 135

static char map[GSZ][GSZ];
static uint8_t reach[GSZ][GSZ][2];	/* time dim. %2 */

int
main(int argc, char **argv)
{
	int p1=0, w,h, x,y,t;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=1; fgets(map[y]+1, sizeof(map[y])-2, stdin); y++) {
		assert(map[y][strlen(map[y]+1)] == '\n');
		assert(y+1 < GSZ-1);
	}

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		if (map[y][x] == 'S') {
			reach[y][x][0] = 1;
			break;
		}

	for (t=0; t<64; t++)
	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		reach[y][x][(t+1)&1] =
		    (map[y][x] == '.' || map[y][x] == 'S') &&
		    (reach[y][x-1][t&1] || reach[y-1][x][t&1] ||
		     reach[y][x+1][t&1] || reach[y+1][x][t&1]);

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		p1 += reach[y][x][t&1];

	printf("21: %d\n", p1);
	return 0;
}
