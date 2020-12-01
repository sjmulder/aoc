#include <stdio.h>

#define MAXNUMS 256

static short nums[MAXNUMS];

int
main(int argc, char **argv)
{
	int count=0, i, j;
	int nsteps=0;

	while (scanf(" %hd", &nums[count]) == 1)
		if (++count >= MAXNUMS) {
			fputs("nums[] overflow\n", stderr);
			return 1;
		}

	for (i=0; i < count-1; i++)
	for (j=i+1; j < count; j++) {
		nsteps++;
		if (nums[i] + nums[j] == 2020) {
			printf("%d\n", nums[i] * nums[j]);
			return 0;
		}
	}

	fputs("no match\n", stderr);
	return 1;
}
