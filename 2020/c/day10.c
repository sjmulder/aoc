#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CAP 128

static int
cmp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

int
main(int argc, char **argv)
{
	static int nums[CAP];
	static long npaths[CAP];
	FILE *f;
	int i,j, len=1, n1=0,n3=1;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while (fscanf(f, " %d", &nums[len]) == 1)
		{ len++; assert(len < CAP); }

	qsort(nums, len, sizeof(nums[0]), cmp);

	for (i=1; i<len; i++)
		switch (nums[i]-nums[i-1]) {
			case 1: n1++; break;
			case 3: n3++; break;
		}

	npaths[len-1] = 1;
	for (i=len-2; i>=0; i--)
		for (j=i+1; j<len && nums[j]-nums[i] <4; j++)
			npaths[i] += npaths[j];
	
	printf("%d %ld\n", n1*n3, npaths[0]);
	//getchar();
	return 0;
}
