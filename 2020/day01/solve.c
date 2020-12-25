#include <stdio.h>
#include <assert.h>

#define MAXNUMS 200000
#define TARGET 2020

int
main(int argc, char **argv)
{
	static short nums[MAXNUMS];
	static char map[TARGET];

	FILE *f;
	int count=0, num,sum, i,j;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while (fscanf(f, " %d", &num) == 1) {
		assert(count < MAXNUMS);
		assert(num >=0 && num < TARGET);
		nums[count++] = num;
		map[num] = 1;
	}

	for (i=0; i < count-1; i++)
		if (map[TARGET-nums[i]]) {
			printf("%d ", nums[i] * (TARGET-nums[i]));
			break;
		}
			
	for (i=0; i < count-1; i++)
	for (j=i+1; j < count; j++) {
		if ((sum = nums[i]+nums[j]) >= TARGET)
			continue;
		if (map[TARGET-sum]) {
			printf("%d\n", nums[i]*nums[j]*(TARGET-sum));
			getchar();
			return 0;
		}
	}
;
	return 1;
}
