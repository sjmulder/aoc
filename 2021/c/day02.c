#include <stdio.h>

int
main()
{
	/* d is pt.2 depth, a is pt.1 depth AND aim */
	char cmd[16];
	int x, h=0,d=0,a=0;

	while (scanf(" %15s %d", cmd, &x) == 2)
		switch (cmd[0]) {
		case 'd': a+=x; break;
		case 'u': a-=x; break;
		case 'f': h+=x; d+=a*x; break;
		default: fprintf(stderr, "bad input\n"); return 1;
		}

	printf("02: %d %d\n", h*a, h*d);
	return 0;
}
