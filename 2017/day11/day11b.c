#define MAX(a, b) ((a)>(b)?(a):(b))

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Usage: day11b <input.txt
   See puzzle.txt, day11a */ 

int
main()
{
	int c, x=0, y=0, dy, maxdist=0;

	while (1) {
		while (isspace(c = getchar()))
			;
		switch (c) {
			case 'n': dy =  1; break;
			case 's': dy = -1; break;
			case EOF: goto done;
			default: goto edir;
		}
		switch ((c = getchar())) {
			case 'w': x--; c = getchar(); break;
			case 'e': x++; c = getchar(); break;
			/* straight up or down is 2 steps */
			default: dy *= 2; break;
		}

		y += dy;
		maxdist = MAX(maxdist, abs(x) + MAX(0, abs(y)-abs(x)) / 2);

		while (isspace(c))
			c = getchar();
		if (c != EOF && c != ',')
			goto edir;
	}
done:
	printf("%d\n", maxdist);
	return 0;
edir:
	fprintf(stderr, "invalid input: '%c' (0x%x)\n", c, c);
	return 1;
}
