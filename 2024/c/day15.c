#include "common.h"

#define GZ 52

static char g[GZ][GZ];
static int px,py;

static void
dump(void)
{
	int x,y;

	for (y=0; y<GZ && isprint(g[y][0]); y++) {
		for (x=0; x<GZ && isprint(g[y][x]); x++)
			putchar(x==px && y==py ? '@' : g[y][x]);
		putchar('\n');
	}

	putchar('\n');
}

static int
push(int x0, int y0, int dx, int dy)
{
	int x,y;

	//printf("pushing from %d,%d to %d,%d\n", x0,y0, dx,dy);

	for (x=x0, y=y0; x>=0 && x<GZ && y>=0 && y<GZ; x+=dx, y+=dy)
		switch (g[y][x]) {
		case '.': g[y0][x0]='.'; g[y][x]='O'; return 1;
		case 'O': break;
		default: return 0;
		}
	
	return 0;
}

static void
move(int dx, int dy)
{
	int px1,py1;

	px1 = px+dx;
	py1 = py+dy;

	assert(px>=0); assert(px<GZ);
	assert(py>=0); assert(py<GZ);
	assert(px1>=0); assert(px1<GZ);
	assert(py1>=0); assert(py1<GZ);

	if (g[py1][px1] == '.' || push(px1,py1, dx,dy))
		{ px=px1; py=py1; }
}

static int
score(void)
{
	int acc=0, x,y;

	for (y=0; y<GZ && g[y][0]; y++)
	for (x=0; x<GZ && g[y][x]; x++)
		if (g[y][x] == 'O')
			acc += 100*y + x;

	return acc;
}

int
main(int argc, char **argv)
{
	int p1=0,p2=0, y, c;
	char *p;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (y=0; fgets(g[y], GZ, stdin); y++) {
		assert(y+1 < GZ);
		if (!g[y][0] || g[y][0]=='\n')
			break;
		if ((p = strchr(g[y], '@')))
			{ py=y; px=p-g[y]; *p='.'; }
	}

	//dump();
	printf("at %d,%d\n", px,py);

	while ((c = getchar()) != EOF) {
		//printf("move: %c\n", c);

		switch (c) {
		case '^': move(0,-1); break;
		case 'v': move(0, 1); break;
		case '<': move(-1,0); break;
		case '>': move( 1,0); break;
		}

		//dump();
	}

	p1 = score();

	printf("15: %d %d\n", p1, p2);
	return 0;
}
