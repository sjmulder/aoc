#include <stdio.h>

int
main(void)
{
	int c, x = 1, y = 1;

	while ((c = getchar()) != -1) {
		switch (c) {
		case 'U': if (y)   y--; break;
		case 'D': if (y<2) y++; break;
		case 'L': if (x)   x--; break;
		case 'R': if (x<2) x++; break;

		case '\n':
			if (x>=0 && x<3 && y>=0 && y<3)
				putchar('1' + y*3 + x);
			break;

		default:
			fprintf(stderr, "invalid input: '%c'\n", (char)c);
			break;
		}
	}

	putchar('\n');
	return 0;
}
