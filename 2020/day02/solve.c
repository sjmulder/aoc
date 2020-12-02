#include <stdio.h>

int
main()
{
	int part1=0, part2=0, lo, hi, count, i;
	char ch, pass[32];

	while (scanf(" %d-%d %c: %31s", &lo, &hi, &ch, pass) == 4) {
		for (count=0, i=0; pass[i]; i++)
			count += pass[i] == ch;
		part1 += count >= lo && count <= hi;
		part2 += (pass[lo-1] == ch) ^ (pass[hi-1] == ch);
	}

	printf("%d %d\n", part1, part2);
}
