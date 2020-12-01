#include <stdio.h>
#include <stdlib.h>

#define MAXNUMS 256

static short nums[MAXNUMS];

static int
cmp_short(const void *a, const void *b)
{
	return *(short *)a - *(short *)b;
}

int
main(int argc, char **argv)
{
	int count=0, i, j, k;
	int nsteps=0;
	short sum;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	fprintf(stderr, "read %d nums\n", count);

	qsort(nums, count, sizeof(nums[0]), cmp_short);

	for (i=0; i < count-2; i++)
	for (j=1; j < count-1; j++)
	for (k = count-1; k>j; k--) {
		nsteps++;
		sum = nums[i] + nums[j] + nums[k];
		if (sum < 2020)
			break;
		if (sum == 2020) {
			fprintf(stderr, "[%d]=%hd [%d]=%hd [%d]=%hd\n",
			    i, nums[i], j, nums[j], k, nums[k]);
			fprintf(stderr, "%d steps\n", nsteps);
			printf("%d\n", nums[i] * nums[j] * nums[k]);
			return 0;
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
