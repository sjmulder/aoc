#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main()
{
	static int a[1001];
	int n=0,min=0,max=0, p1=0,p2=0, sum1,sum2,pos,d,i;

	for (; scanf("%d,", &a[n]) == 1; n++) {
		if (!n || a[n]<min) min = a[n];
		if (!n || a[n]>max) max = a[n];
	}

	for (pos=min; pos<=max; pos++) {
		for (i=sum1=sum2=0; i<n; i++) {
			d = abs(pos-a[i]);
			sum1 += d;
			sum2 += d*(d+1)/2;
		}
		if (pos==min || sum1<p1) p1 = sum1;
		if (pos==min || sum2<p2) p2 = sum2;
	}

	printf("07: %d %d\n", p1, p2);
	return 0;
}
