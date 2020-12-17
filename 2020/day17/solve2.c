#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define SZ 30

static char gr[SZ][SZ][SZ][SZ];

static void
parse(void)
{
	int x,y,z,w, c;

	x=y=z=w = SZ/2;

	for (; (c = getchar()) != EOF; x++)
		if (c == '#')
			gr[w][z][y][x] = 1;
		else if (c == '\n')
			{ x = SZ/2-1; y++; }
}

static void
step(void)
{
	static char nadj[SZ][SZ][SZ][SZ];
	int x,y,z,w, dx,dy,dz,dw;

	memset(nadj, 0, sizeof(nadj));

	for (dw=-1; dw<2; dw++)
	for (dz=-1; dz<2; dz++)
	for (dy=-1; dy<2; dy++)
	for (dx=-1; dx<2; dx++) {
		if (!dx && !dy && !dz && !dw)
			continue;
		for (w=1; w<SZ-1; w++)
		for (z=1; z<SZ-1; z++)
		for (y=1; y<SZ-1; y++)
		for (x=1; x<SZ-1; x++)
			nadj[w][z][y][x] += gr[w+dw][z+dz][y+dy][x+dx];
	}

	for (w=1; w<SZ-1; w++)
	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		gr[w][z][y][x] =
		    (nadj[w][z][y][x] == 2 && gr[w][z][y][x]) ||
		     nadj[w][z][y][x] == 3;
}

static int
counton(void)
{
	int x,y,z,w, non=0;

	for (w=1; w<SZ-1; w++)
	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		non += gr[w][z][y][x];

	return non;
}

int
main()
{
	int i;

	parse();
	for (i=0; i<6; i++)
		step();
	printf("%d\n", counton());
}
