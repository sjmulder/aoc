#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define PRESZ 25
#define CAP 2048

int
main(int argc, char **argv)
{
	static long ns[CAP];
	FILE *f;
	long target=0, start=0,end=0,sum=0, min=LONG_MAX,max=LONG_MIN;
	int i,j,k, len=0;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while (fscanf(f, " %ld", &ns[len]) == 1) {
		len++;
		assert(len < CAP);
	}
	
	for (i=PRESZ; !target && i<len; i++) {
		for (j=i-PRESZ; j<i; j++)
		for (k=i-PRESZ; k<j; k++)
			if (ns[j]+ns[k] == ns[i])
				goto good;

		target = ns[i];
	good:	;
	}

	while (sum != target || end-start <= 2) {
		while (sum < target)
			{ sum += ns[end++]; assert(end<CAP); }
		while (sum > target)
			{ sum -= ns[start++]; assert(start<CAP); }
	}

	for (i=start; i<=end; i++) {
		min = ns[i]<min ? ns[i] : min;
		max = ns[i]>max ? ns[i] : max;
	}

	printf("%ld %ld\n", target, min+max);
	//getchar();
	return 0;
}
