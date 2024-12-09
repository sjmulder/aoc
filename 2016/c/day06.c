#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>

int
main(int argc, char **argv)
{
	char buf[16];
	int max_j=0, max_c=0, counts[8][256]={}, i,j;
	int min_j=0, min_c=0;

	if (argc > 1)
		freopen(argv[1], "r", stdin);

	while (fgets(buf, sizeof(buf), stdin))
		for (i=0; i<8 && isalnum(buf[i]); i++)
			counts[i][(int)buf[i]]++;
	
	printf("06: ");

	/* TODO: single pass for min and max into ans[] strings */

	for (i=0; i<8; i++) {
		for (j='a', max_c=0; j <= 'z'; j++)
			if (counts[i][j] > max_c)
				{ max_j = j; max_c = counts[i][j]; }
		putchar(max_j);
	}

	putchar(' ');

	for (i=0; i<8; i++) {
		for (j='a', min_c=INT_MAX; j <= 'z'; j++)
			if (counts[i][j] < min_c)
				{ min_j = j; min_c = counts[i][j]; }
		putchar(min_j);
	}

	putchar('\n');
	return 0;
}
