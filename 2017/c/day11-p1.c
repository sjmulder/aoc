#define MAX(a, b) ((a)>(b)?(a):(b))

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Usage: day11a <input.txt
   See puzzle.txt

   Method: hex grid is mapped to 2-dimensional grid (see ASCII illustration
   below). The solution is calculated as the number of diagonal steps plus the
   number of remaining vertical steps.

     ...     _   _
      1 _ \_/ \_/ \/
   y  0 _ / \_/ \_/\_
     -1 _ \_/ \_/ \/
     ...  / \_/ \_/\_
             | | |
         ...-1 0 1...
               x
*/

int
main()
{
	int c, x=0, y=0, dy, ndiag, nvert;

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
		while (isspace(c))
			c = getchar();
		if (c != EOF && c != ',')
			goto edir;	
	}
done:
	ndiag = abs(x);
	nvert = MAX(0, abs(y)-abs(x)) / 2;
	printf("(%d,%d) -> %d+%d = %d\n", x, y, ndiag, nvert, ndiag + nvert);
	return 0;
edir:
	fprintf(stderr, "invalid input: '%c' (0x%x)\n", c, c);
	return 1;
}
