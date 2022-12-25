#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * INCORRECT attempt at solving part 2 with recursion.
 *
 * The problem here is that resolve() too quickly concludes that there is
 * no route to EXPOSED. This happens when somewhere in the recursion all
 * the surrounding nodes are CUBE or PENDING, it concludes this must be
 * AIR (pocket) then, and that's propagated back - even though another path
 * of the original recursion may have found EXPOSED.
 *
 * See for example the output for coord '6 19 12', which *should* be
 * EXPOSED:
 *
 *   6 19 12 ??
 *    5 19 12 CUBE
 *    7 19 12 ??
 *     6 19 12 PENDING
 *     8 19 12 ??
 *      7 19 12 PENDING
 *      9 19 12 CUBE
 *      8 18 12 CUBE
 *      8 20 12 CUBE
 *      8 19 11 CUBE
 *      8 19 13 CUBE
 *     AIR then         <-- foregone conclusion
 *    AIR (copied)
 *   AIR (copied)
 */

#define GSZ	24

static char grid[GSZ][GSZ][GSZ];
static const int d[6][3] = {{-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0,-1},     {0,0,1}};

#define PENDING	1
#define CUBE	2
#define AIR	3
#define EXPOSED	4

static const char * const names[] = {"??", "PENDING", "CUBE", "AIR",
    "EXPOSED"};

static int
resolve(int x, int y, int z)
{
	static int depth=1;
	int i;
	char val;

	if (x<0 || x>=GSZ || y<0 || y>=GSZ || z<0 || z>=GSZ) {
		for (i=0; i<depth; i++) putchar(' ');
		printf("%d %d %d EXPOSED (grid edge)\n", x,y,z);
		return EXPOSED;
	}

	for (i=0; i<depth; i++) putchar(' ');
	printf("%d %d %d %s\n", x,y,z, names[(int)grid[z][y][x]]);

	if (grid[z][y][x]) {
		//for (i=0; i<depth; i++) putchar(' ');
		//printf("returning it\n");
		//printf("%s (known)\n", names[(int)grid[z][y][x]]);
		return grid[z][y][x];
	}

	grid[z][y][x] = PENDING;

	for (i=0; i<6; i++) {
		depth++;
		if ((val = resolve(x+d[i][0], y+d[i][1], z+d[i][2]))>CUBE) {
			depth--;
			for (i=0; i<depth; i++) putchar(' ');
			printf("%s (copied)\n", names[(int)val]);
			return grid[z][y][x] = val;
		}
		depth--;
	}

	for (i=0; i<depth; i++) putchar(' ');
	printf("AIR then\n");
	return grid[z][y][x] = AIR;
}

int
main()
{
	int x,y,z, i, p1=0,p2=0, val;

	//(void)!freopen("../data/18-input.txt", "r", stdin);

	while (scanf(" %d,%d,%d", &x,&y,&z) == 3) {
		assert(x>=0); assert(x<GSZ);
		assert(y>=0); assert(y<GSZ);
		assert(z>=0); assert(z<GSZ);

		grid[z][y][x] = CUBE;
	}

	for (z=0; z<GSZ; z++)
	for (y=0; y<GSZ; y++)
	for (x=0; x<GSZ; x++) {
		if (grid[z][y][x] != CUBE)
			continue;
		printf("%d %d %d\n", x,y,z);
		for (i=0; i<6; i++) {
			val = resolve(x+d[i][0], y+d[i][1], z+d[i][2]);
			p1 += val >= AIR;
			p2 += val >= EXPOSED;
		}
	}

	for (x=0; x<GSZ; x++)
	for (y=0; y<GSZ; y++)
	for (z=0; z<GSZ; z++)
		printf("%d %d %d %s\n", x,y,z, names[(int)grid[z][y][x]]);

	printf("18: %d %d\n", p1, p2);
	return 0;
}
