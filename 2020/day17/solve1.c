#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define SZ 50

static char gr[SZ][SZ][SZ];
static long non;

static void
parse(void)
{
	int x=SZ/2, y=SZ/2, z=SZ/2, c;

	for (; (c = getchar()) != EOF; x++)
		if (c == '#')
			{ gr[z][y][x] = 1; non++; }
		else if (c == '\n')
			{ x = SZ/2-1; y++; }

	printf(" (parse) %ld\n", non);
}

static void
step(void)
{
	static char nadj[SZ][SZ][SZ];
	int x,y,z, x2,y2,z2;

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

	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
	for (z2=z-1; z2<z+2; z2++)
	for (y2=y-1; y2<y+2; y2++)
	for (x2=x-1; x2<x+2; x2++)
		if ((z!=z2 || y!=y2 || x!=x2) && gr[z2][y2][x2])
			nadj[z][y][x]++;

	for (z=1; z<SZ-1; z++)
	for (y=1; y<SZ-1; y++)
	for (x=1; x<SZ-1; x++)
		if ((nadj[z][y][x] == 2 && gr[z][y][x]) ||
		     nadj[z][y][x] == 3) {
			if (gr[z][y][x])
				continue;
			if (z==1 || y==1 || x==1 ||
			    z==SZ-2 || y==SZ-2 || x==SZ-2)
				errx(1, "going out of bounds");
			gr[z][y][x] = 1;
			non++;
		} else {
			if (!gr[z][y][x])
				continue;
			gr[z][y][x] = 0;
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
