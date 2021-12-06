#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

int
main()
{
	uint64_t ages[10]={0}, p1=0,p2=0;
	int age,day,i;

	while (scanf("%d,", &age) == 1)
		ages[age]++;

	for (day=0; day<=256; day++) {
		ages[7] += ages[0];
		ages[9] = ages[0];
		memmove(ages, ages+1, sizeof(*ages)*9);

		if (day == 80)
			for (i=0; i<8; i++)
				p1 += ages[i];
	}

	for (i=0; i<8; i++)
		p2 += ages[i];

	printf("06: %" PRIu64 " %" PRIu64 "\n", p1, p2);
	return 0;
}
