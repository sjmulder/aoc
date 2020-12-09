#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <err.h>

#define PRESZ 25
#define CAP 1024

int
main()
{
	static long ns[CAP];
	long target=0, min,max, sum;
	int i,j,k, len=0;

	while (scanf(" %ld", &ns[len]) == 1)
		if (++len >= CAP)
			errx(1, "ns overflow");
	
	for (i=PRESZ; !target && i<len; i++) {
		for (j=i-PRESZ; j<i; j++)
		for (k=i-PRESZ; k<j; k++)
			if (ns[j]+ns[k] == ns[i])
				goto good;

		target = ns[i];
	good:	;
	}

	for (i=0; i<len; i++) {
		sum=0; min=LONG_MAX; max=LONG_MIN;

		for (j=i-1; sum<target &&  j>=0; j--) {
			min = ns[j]<min ? ns[j] : min;
			max = ns[j]>max ? ns[j] : max;
			sum += ns[j];
		}

		if (sum == target && j < i-1 )
			break;
	}

	printf("%ld %ld\n", target, min+max);
}
