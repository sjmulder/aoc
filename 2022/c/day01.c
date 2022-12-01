#include <stdio.h>
#include <string.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

int
main()
{
	char buf[64], *line;
	int cals, sum=0, top1=0, top[3]={}, top_sum=0;
	size_t i;

	do {
		line = fgets(buf, sizeof(buf), stdin);

		if (line && sscanf(line, "%d", &cals) == 1)
			sum += cals;
		else {
			for (i=0; i < LEN(top); i++)
				if (top[i] < sum) {
					top_sum += sum - top[LEN(top)-1];
					memmove(&top[i+1], &top[i],
					   sizeof(*top) * (LEN(top)-1-i));
					top[i] = sum;
					break;
				}
			if (top1 < sum)
				top1 = sum;
			sum = 0;
		}
	} while (line);

	printf("%d %d\n", top1, top_sum);

	return 0;
}
