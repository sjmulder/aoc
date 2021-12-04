#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define SZ 5

static int draws[100];
static int boards[100][5][5];
static int struck[100][5][5];
static int ndraws, nboards;

static void
dump(void)
{
	int i,x,y;

	for (i=0; i<ndraws; i++)
		printf("%d ", draws[i]);
	printf("\n\n");

	for (i=0; i<nboards; i++) {
		for (y=0; y<SZ; y++) {
			for (x=0; x<SZ; x++)
				printf("%2d%c ",
				    boards[i][y][x],
				    struck[i][y][x] ? 'X' : ' ');
			printf("\n");
		}
		printf("\n");
	}
}

int
main()
{
	char buf[400], *rest, *lf, *field;
	int i,j, x,y, x2,y2, winner=-1, score=0;

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
		for (x2=0; x2<SZ && struck[j][y][x2]; x2++) ;
		for (y2=0; y2<SZ && struck[j][y2][x]; y2++) ;
		if (x2 == SZ || y2 == SZ)
			{ winner = j; goto end; }
	}

end:
	if (winner == -1)
		errx(1, "no winner");

	for (y=0; y<SZ; y++)
	for (x=0; x<SZ; x++)
		score += !struck[winner][y][x] * boards[winner][y][x];
	score *= draws[i];

	printf("04: %d\n", score);
	return 0;
}
