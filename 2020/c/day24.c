#include <stdio.h>
#include <string.h>
#include <assert.h>

#define GSZ	196
#define GMID	(GSZ/2)
#define NROUNDS	100

static char grid[GSZ][GSZ];
static int nset;

static void
step(void)
{
	static const int dx[] = {-1, 0,-1, 1, 0, 1};
	static const int dy[] = {-1,-1, 0, 0, 1, 1};
	static char nadj[GSZ][GSZ];
	int i,x,y;

	memset(nadj, 0, sizeof(nadj));

	for (i=0; i<6; i++)
	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++)
		nadj[y][x] += grid[y+dy[i]][x+dx[i]];

	for (y=1; y<GSZ-1; y++)
	for (x=1; x<GSZ-1; x++) {
		nset -= grid[y][x];
		nset += grid[y][x] =
		     (nadj[y][x]==1 && grid[y][x]) ||
		      nadj[y][x]==2;
		if (grid[y][x]) {
			assert(x>1); assert(x<GSZ-2);
			assert(y>1); assert(y<GSZ-2);
		}
	}
}

int
main(int argc, char **argv)
{
	FILE *f;
	int i, c=0,c2, x=GMID,y=GMID;

	f = argc > 1 ? fopen(argv[1], "r") : stdin;
	assert(f);

	while ((c = fgetc(f)) != EOF) {
		switch (c) {
		case 'n': y--; break;
		case 's': y++; break;
		case 'w': x--; break;
		case 'e': x++; break;
		case '\n':
			assert(x>0); assert (y<GSZ-1);
			assert(x>0); assert (y<GSZ-1);
			nset -= grid[y][x];
			nset += grid[y][x] = !grid[y][x];
			x = y = GMID;
			break;
		default:
			assert(0 && "bad direction");
			break;
		}

		if (c=='n' || c=='s')
			switch ((c2 = fgetc(f))) {
			case 'w': if (c!='s') x--; break;
			case 'e': if (c!='n') x++; break;
			default: ungetc(c2, f); break;
			}
	}

	printf("%d ", nset);

	for (i=0; i<NROUNDS; i++)
		step();

	printf("%d\n", nset);
	//getchar();
	return 0;
}
