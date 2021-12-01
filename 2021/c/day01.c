#include <stdio.h>
#include <string.h>

int
main()
{
	int a[4] = {0}, n=0, ninc1=0, ninc3=0;

	for (; scanf(" %d", a) == 1; n++) {
		ninc1 += n>0 && a[0] > a[1];
		ninc3 += n>2 && a[0]+a[1]+a[2] > a[1]+a[2]+a[3];
		memmove(a+1, a, sizeof(int)*3);
	}

	printf("%d %d\n", ninc1, ninc3);
}
