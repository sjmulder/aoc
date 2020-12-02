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
main(int argc, char **argv)
{
	int count=0, i, j;
	short sum;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	qsort(nums, count, sizeof(nums[0]), cmp_short);

	for (i=0; i < count-1; i++)
	for (j = count-1; j; j--) {
		sum = nums[i] + nums[j];
		if (sum < 2020)
			break;
		if (sum == 2020) {
			printf("%d\n", nums[i] * nums[j]);
			return 0;
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
