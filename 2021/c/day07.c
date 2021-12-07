#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

int
main()
{
	static int a[1001];
	int n=0, min=INT_MAX, max=INT_MIN;
	int p1=INT_MAX, p2=INT_MAX, sum1,sum2,pos,d,i;

	while (scanf("%d,", &a[n]) == 1) {
		if (a[n] < min) min = a[n];
		if (a[n] > max) max = a[n];
		n++;
		assert(n < (int)LEN(a));
	}

	for (pos=min; pos<=max; pos++) {
		for (sum1=sum2=0, i=0; i<n; i++) {
			d = abs(pos-a[i]);
			sum1 += d;
			sum2 += d*(d+1)/2;
		}
		if (sum1 < p1) p1 = sum1;
		if (sum2 < p2) p2 = sum2;
	}

	printf("07: %d %d\n", p1, p2);
}
