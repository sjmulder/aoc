#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
main()
{
	int c;
	int64_t acc=0, sum=0;
	char buf[128], *b;

	while ((c = getchar()) != EOF)
		switch(c) {
		case '=': acc *= 5; acc +=-2; break;
		case '-': acc *= 5; acc +=-1; break;
		case '0': acc *= 5; acc += 0; break;
		case '1': acc *= 5; acc += 1; break;
		case '2': acc *= 5; acc += 2; break;
		case '\n': sum += acc; acc = 0; break;
		}

	b = &buf[sizeof(buf)-1];
	*b = '\0';

	sum += 2;

	while (sum) {
		switch (sum%5) {
		case 0: *--b = '='; break;
		case 1: *--b = '-'; break;
		case 2: *--b = '0'; break;
		case 3: *--b = '1'; break;
		case 4: *--b = '2'; break;
		}
		printf("%" PRId64 "\n", sum);
		sum /= 5;
	}

	printf("25: %s", b);

	return 0;
}
