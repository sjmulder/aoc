#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define SZ 5

static int draws[100];
static int boards[100][5][5];
static int struck[100][5][5];
static int won[100];
static int ndraws, nboards, nwon;

static int
score(int board, int lastdraw)
{
	int sum=0, x,y;

	for (y=0; y<SZ; y++)
	for (x=0; x<SZ; x++)
		sum += !struck[board][y][x] * boards[board][y][x];

	return sum * lastdraw;
}

int
main()
{
	char buf[400], *rest, *lf, *field;
	int i,j, x,y, x2,y2, p1=0,p2=0;

	if (!(rest = fgets(buf, sizeof(buf), stdin)))
		err(1, NULL);
	if ((lf = strchr(buf, '\n')))
		*lf = '\0';
	while ((field = strsep(&rest, ","))) {
		if (ndraws >= (int)LEN(draws))
			errx(1, "draws overflow");
		draws[ndraws++] = atoi(field);
	}
	while (!feof(stdin)) {
		if (nboards >= (int)LEN(boards))
			errx(1, "boards overflow");
		for (y=0; y<SZ; y++)
		for (x=0; x<SZ; x++)
			if (scanf(" %d ", &boards[nboards][y][x]) != 1)
				errx(1, "not a full board");
		nboards++;
	}

	for (i=0; i<ndraws; i++)
	for (j=0; j<nboards; j++)
	for (y=0; y<SZ; y++)
	for (x=0; x<SZ; x++) {
		if (struck[j][y][x]) continue;
		if (boards[j][y][x] != draws[i]) continue;
		struck[j][y][x] = 1;
		if (won[j]) continue;
		for (x2=0; x2<SZ && struck[j][y][x2]; x2++) ;
		for (y2=0; y2<SZ && struck[j][y2][x]; y2++) ;
		if (x2 != SZ && y2 != SZ) continue;
		won[j] = 1;
		if (++nwon == 1) p1 = score(j, draws[i]);
		if (nwon == nboards) { p2 = score(j, draws[i]);
		    goto end; }
	}

end:
	printf("04: %d %d\n", p1, p2);
	return 0;
}
