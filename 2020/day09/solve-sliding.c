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
	long target=0, start=0,end=0,sum=0, min=LONG_MAX,max=LONG_MIN;
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

	while (sum != target || end-start <= 2) {
		while (sum < target) sum += ns[end++];
		while (sum > target) sum -= ns[start++];
	}

	for (i=start; i<=end; i++) {
		min = ns[i]<min ? ns[i] : min;
		max = ns[i]>max ? ns[i] : max;
	}

	printf("%ld %ld\n", target, min+max);
}
