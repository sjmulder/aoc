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
main()
{
	/* Looks like this is less efficient than solve2-sort */

	int count=0, i, j, k;
	int k_min, k_max;
	int nsteps=0;
	short sum;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	fprintf(stderr, "read %d nums\n", count);

	qsort(nums, count, sizeof(nums[0]), cmp_short);

	for (i=0; i < count-1; i++)
	for (j = count-1; j > i+1; j--) {
		if (nums[i] + nums[j] >= 2020)
			continue;

		k_min = i+1;
		k_max = j-1;
		while (k_min <= k_max) {
			nsteps++;
			k = k_min + (k_max-k_min)/2;
			sum = nums[i] + nums[j] + nums[k];

			if (sum < 2020)
				k_min = k+1;
			else if (sum > 2020)
				k_max = k-1;
			else {
				fprintf(stderr, "%d steps\n", nsteps);
				printf("%d\n", nums[i] * nums[j] *
				    nums[k]);
				return 0;
			}
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
