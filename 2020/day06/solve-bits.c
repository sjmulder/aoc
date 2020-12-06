#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main() {
	int c=0, prev, p1=0, p2=0, gpsz=0;
	uint32_t mask=0, any=0, all=(1<<26)-1;

	while ((prev = c) != EOF)
		if ((c = getchar()) != '\n' && c != EOF)
			mask |= 1 << (c-'a');
		else if (prev == '\n') {
			p1 += __builtin_popcount(any);
			p2 += __builtin_popcount(all);
			gpsz=0; any=0; all=(1<<26)-1;
		} else
			{ any |= mask; all &= mask; mask=0; gpsz++; }

	printf("%d %d\n", p1, p2);
}
