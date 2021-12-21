#include <stdio.h>

/*
 * a[] is a 4-element ring buffer. Multiplication is used instead of
 * '&&' to make this branchless. The '(n+3)%4' actually means '(n-1)%4'
 * but that would yield some negative indices.
 */

int
main()
{
	int a[4] = {0}, n=0, ninc1=0, ninc3=0;

	for (; scanf(" %d", &a[n%4]) == 1; n++) {
		ninc1 += (n>0) * (a[n%4] > a[(n+3)%4]);
		ninc3 += (n>2) * (a[n%4] > a[(n+1)%4]);
	}

	printf("01: %d %d\n", ninc1, ninc3);
	return 0;
}
