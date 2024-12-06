#include "common.h"

#define GZ 134

static const char carets[] = "^>v<";
static const int dx[4] = {0,1,0,-1}, dy[4] = {-1,0,1,0};
static char g0[GZ][GZ], g[GZ][GZ];	/* orig & cur grid */
static int cx0,cy0,dir0;		/* orig pos */
static int cx,cy,dir;			/* current pos */

static void
dump(void)
{
	int x,y;

	for (y=0; y<45; y++) {
		for (x=0; x<80; x++)
			putchar(isprint(g[y][x]) ? g[y][x] : '~');
		putchar('\n');
	}
}

static int
trace(int *loop)
{
	int n=0;

	if (loop) *loop = 0;

	while (g[cy][cx]=='.' || *strchr(carets, g[cy][cx])) {
		printf("cy %d, cx %d", cy, cx);
		printf("%c %d %c\n", g[cy][cx], dir, carets[dir]);
		if (g[cy][cx] == carets[dir])
			{ puts("yes"); if (loop) *loop = 1; break; }
		if (g[cy][cx] == '.')
			{ g[cy][cx] = carets[dir]; n++; }
		while (g[cy+dy[dir]][cx+dx[dir]] == '#')
			dir = (dir+1)%4;
		cx += dx[dir];
		cy += dy[dir];
		dump();
		printf("cx %d, cy %d, dir %d\n", cx, cy, dir);
	}

	return n;
}

int
main(int argc, char **argv)
{
	int p1=1,p2=0, x,y,i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	for (y=1; y<GZ && fgets(g0[y]+1, GZ-1, stdin); y++)
		;

	for (y=0; y<GZ; y++)
	for (x=0; x<GZ; x++)
	for (i=0; i<4; i++)
		if (g0[y][x] == carets[i])
			{ cx0=x; cy0=y; dir0=i; goto found_start; }
	
found_start:
	printf("cy0 %d, cx0 %d", cy0, cx0);
	g0[cy0][cx0] = '.';

	memcpy(g, g0, sizeof(g)); cx=cx0; cy=cy0; dir=dir0;
	dump();
	p1 = trace(NULL);

	printf("05: %d %d\n", p1, p2);
	return 0;
}
