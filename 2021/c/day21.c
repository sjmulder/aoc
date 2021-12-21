#include <stdio.h>

#define MIN(a,b) ((a)<(b)?(a):(b))

int
main(void)
{
	int pos[2], score[2]={0}, nthrows=0;
	int i,j,p, throw;

	scanf(" Player 1 starting position: %d"
	      " Player 2 starting position: %d", pos, pos+1);

	for (i=0; ; i++)
	for (p=0; p<2; p++) {
		for (throw=0, j=0; j<3; j++)
			throw += nthrows++ % 100 +1;

		pos[p] = (pos[p] + throw -1) % 10 +1;
		score[p] += pos[p];

		if (score[p] >= 1000)
			goto end;
	}
end:
	printf("21: %d\n", MIN(score[0], score[1]) * nthrows);
	return 0;
}
