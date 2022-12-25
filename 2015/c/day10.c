#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define BUFSZ 4*1024*1024

int
main(int argc, char **argv)
{
	static char bufs[2][BUFSZ];
	char *s,*d;
	int n,i, si,di, digit,count;

	if (argc != 3)
		errx(1, "usage: solve <input> <N>");
	if (snprintf(bufs[0], BUFSZ, "%s", argv[1]) > BUFSZ)
		errx(1, "input too long");
	if ((n = atoi(argv[2])) < 1)
		errx(1, "bad N");

	for (i=0; i<n; i++) {
		s = bufs[i&1];
		d = bufs[!(i&1)];
		di=0; count=1; digit=s[0];

		for (si=1; s[si-1]; si++)
			     if (s[si] == digit) count++;
			else if (count > 9)      errx(1, "count overflow");
			else if (di > BUFSZ-3)   errx(1, "buf overflow");
			else {
				d[di++] = '0'+count;
				d[di++] = digit;
				count=1; digit=s[si];
			}

		d[di] = '\0';
	}

	printf("%zu\n", strlen(d));
	return 0;
}
