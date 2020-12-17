#include <stdio.h>
#include <string.h>

#define SZ 30

static char gr[SZ][SZ][SZ];

static void
parse(void)
{
	int x=SZ/2,y=SZ/2,z=SZ/2, c;

	for (; (c = getchar()) != EOF; x++)
		     if (c == '#')  gr[z][y][x] = 1;
		else if (c == '\n') { x = SZ/2-1; y++; }
}

static void
step(void)
{
	static char nadj[SZ][SZ][SZ];
	int x,y,z, dx,dy,dz;

	memset(nadj, 0, sizeof(nadj));

	for (dz=-1; dz<2; dz++)
	for (dy=-1; dy<2; dy++)
	for (dx=-1; dx<2; dx++) {
		if (!dx && !dy && !dz)
			continue;
		for (z=1; z<SZ-1; z++)
		for (y=1; y<SZ-1; y++)
		for (x=1; x<SZ-1; x++)
			nadj[z][y][x] += gr[z+dz][y+dy][x+dx];
	}

	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		gr[z][y][x] =
		    (nadj[z][y][x] == 2 && gr[z][y][x]) ||
		     nadj[z][y][x] == 3;
}

static int
counton(void)
{
	int x,y,z, non=0;

	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		non += gr[z][y][x];

	return non;
}

int
main()
{
	int i;

	parse();
	for (i=0; i<6; i++) step();
	printf("%d\n", counton());
}
