#include <stdio.h>

int
main()
{
	char buf[64], *line;
	int cals, sum=0, top[3]={};

	do {
		line = fgets(buf, sizeof(buf), stdin);

		if (line && sscanf(line, "%d", &cals) == 1)
			sum += cals;
		else {
			if (sum > top[0]) {
				top[2] = top[1];
				top[1] = top[0];
				top[0] = sum;
			} else if (sum > top[1]) {
				top[2] = top[1];
				top[1] = sum;
			} else if (sum > top[2])
				top[2] = sum;
			sum = 0;
		}
	} while (line);

	printf("01: %d %d\n", top[0], top[0]+top[1]+top[2]);

	return 0;
}
