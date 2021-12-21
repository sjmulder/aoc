#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Optimised verison using the median and average for part 1 and part 2
 * respectively. Approach totally stolen from Reddit.
 */

static int
compar(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

int
main()
{
	static int a[1001];
	int n=0,sum=0, p1=0,p2=0, cost,pos,d,i;

	while (scanf("%d,", &a[n]) == 1)
		sum += a[n++];

	qsort(a, n, sizeof(*a), compar);

	for (i=0; i<n; i++)
		p1 += abs(n/2-a[i]);

	for (pos=sum/n; pos<=sum/n+1; pos++) {
		for (i=cost=0; i<n; i++) {
			d = abs(pos-a[i]);
			cost += d*(d+1)/2;
		}
		if (!p2 || cost<p2)
			p2 = cost;
	}

	printf("07: %d %d\n", p1, p2);
	return 0;
}
