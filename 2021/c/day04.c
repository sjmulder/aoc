#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compat.h"

#define LEN(a) (sizeof(a)/sizeof(*(a)))
#define SZ 5

int
main()
{
	static int draws[100];
	static int boards[100][SZ][SZ], struck[100][SZ][SZ], won[100];
	int ndraws=0, nboards=0, nwon=0;
	int i,j, x,y, x2,y2, score, p1=0,p2=0;
	char buf[400], *rest, *lf, *field;

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
		if (won[j]) continue;
		if (struck[j][y][x]) continue;
		if (boards[j][y][x] != draws[i]) continue;
		struck[j][y][x] = 1;
		for (x2=0; x2<SZ && struck[j][y][x2]; x2++) ;
		for (y2=0; y2<SZ && struck[j][y2][x]; y2++) ;
		if (x2 != SZ && y2 != SZ) continue;
		won[j] = 1;
		if (++nwon > 1 && nwon < nboards) continue;
		score = 0;
		for (y2=0; y2<SZ; y2++)
		for (x2=0; x2<SZ; x2++)
			score += !struck[j][y2][x2] * boards[j][y2][x2];
		score *= draws[i];
		if (nwon == 1) p1 = score;
		if (nwon == nboards) { p2 = score; goto end; }
	}

end:
	printf("04: %d %d\n", p1, p2);
	return 0;
}
