#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>

#define CAP 1024

int
main(int argc, char **argv)
{
	static long ns[CAP];
	long target, min,max, sum;
	int i,j, len=0;

	if (argc != 2)
		errx(1, "usage: solve2 <target>");
	
	target = strtol(argv[1], NULL, 10);

	while (scanf(" %ld", &ns[len]) == 1)
		if (++len >= CAP)
			errx(1, "ns overflow");
	
	for (i=0; i<len; i++) {
		sum=0; min=LONG_MAX; max=LONG_MIN;

		for (j=i-1; sum<target &&  j>=0; j--) {
			min = ns[j]<min ? ns[j] : min;
			max = ns[j]>max ? ns[j] : max;
			sum += ns[j];
		}

		if (sum == target && j < i-1 )
			{ printf("%ld\n", min+max); return 0; }
	}
}
