#include <stdio.h>
#include <stdlib.h>

#define SZ 25

int
main(int argc, char **argv)
{
	long prev[SZ], n;
	int i,j, sz=SZ, np=0;

	if (argc == 2)
		sz = atoi(argv[1]);

	while (np<sz && scanf(" %ld", &n) == 1)
		prev[np++] = n;

	for (; scanf(" %ld", &n) == 1; prev[np++ %sz] = n) {
		for (i=0; i<sz; i++)
		for (j=0; j<i; j++)
			if (prev[i]+prev[j] == n)
				goto good;

		printf("%ld\n", n);
		return 0;
	good:	;
	}
}
