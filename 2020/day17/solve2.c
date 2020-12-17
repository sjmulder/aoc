#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define SZ 30

static char gr[SZ][SZ][SZ][SZ];
static long non;

static void
parse(void)
{
	int x,y,z,w, c;

	x=y=z=w = SZ/2;

	for (; (c = getchar()) != EOF; x++)
		if (c == '#')
			{ gr[w][z][y][x] = 1; non++; }
		else if (c == '\n')
			{ x = SZ/2-1; y++; }

	printf(" (parse) %ld\n", non);
}

static void
step(void)
{
	static char nadj[SZ][SZ][SZ][SZ];
	int x,y,z,w, x2,y2,z2,w2;

#if 0
	printf("___\n");
	for (z = SZ/2-2; z < SZ/2+3; z++) {
		printf("\nz=%d\n", z);
		for (y = SZ/2-1; y < SZ/2+5; y++) {
			for (x = SZ/2-1; x < SZ/2+5; x++)
				putchar(gr[z][y][x] ? '#' : '.');
			putchar('\n');
		}
	}
#endif

	memset(nadj, 0, sizeof(nadj));

	for (w=1; w<SZ-1; w++)
	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
	for (w2=w-1; w2<w+2; w2++)
	for (z2=z-1; z2<z+2; z2++)
	for (y2=y-1; y2<y+2; y2++)
	for (x2=x-1; x2<x+2; x2++)
		if ((w!=w2||z!=z2||y!=y2||x!=x2) && gr[w2][z2][y2][x2])
			nadj[w][z][y][x]++;

	for (w=1; w<SZ-1; w++)
	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		if ((nadj[w][z][y][x] == 2 && gr[w][z][y][x]) ||
		     nadj[w][z][y][x] == 3) {
			if (gr[w][z][y][x])
				continue;
			if (w==1 || z==1 || y==1 || x==1 ||
			    w==SZ-2 || z==SZ-2 || y==SZ-2 || x==SZ-2)
				errx(1, "going out of bounds");
			gr[w][z][y][x] = 1;
			non++;
		} else {
			if (!gr[w][z][y][x])
				continue;
			gr[w][z][y][x] = 0;
			non--;
		}

	printf(" (step) %ld\n", non);
}

int
main()
{
	int i;

	parse();

	for (i=0; i<6; i++)
		step();

	printf("%ld\n", non);
}
