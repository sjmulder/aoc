#include <stdio.h>
#include <stdlib.h>

#define MAXNUMS 200001

static short nums[MAXNUMS];

static int
cmp_short(const void *a, const void *b)
{
	return *(short *)a - *(short *)b;
}

int
main()
{
	int count=0, i, j, k;
	short sum;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	qsort(nums, count, sizeof(nums[0]), cmp_short);

	for (i=0; i < count-2; i++)
	for (j=i+1; j < count-1; j++)
	for (k = count-1; k>j; k--) {
		sum = nums[i] + nums[j] + nums[k];
		if (sum < 2020)
			break;
		if (sum == 2020) {
			printf("%d\n", nums[i] * nums[j] * nums[k]);
			return 0;
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
