#include <stdio.h>

#define MAXNUMS 256

static short nums[MAXNUMS];

int
main(int argc, char **argv)
{
	int count=0, i, j, k;
	int nsteps=0;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	for (i=0; i < count-2; i++)
	for (j=1; j < count-1; j++)
	for (k=2; k < count; k++) {
		nsteps++;
		if (nums[i] + nums[j] + nums[k] == 2020) {
			fprintf(stderr, "%d steps\n", nsteps);
			printf("%d\n", nums[i] * nums[j] * nums[k]);
			return 0;
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
