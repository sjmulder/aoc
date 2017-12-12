#define MIN(a,b) (a<b ? a : b)

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int nleft, edge=0, x=0, y=0, n;

	if (argc < 2) {
		fprintf(stderr, "no number given\n");
		return 1;
	}

	nleft = (int)strtol(argv[1], NULL, 10) - 1;
	while (nleft) {
		n = MIN(nleft, 1 + edge++/2); nleft -= n; x += n;
		n = MIN(nleft, 1 + edge++/2); nleft -= n; y -= n;
		n = MIN(nleft, 1 + edge++/2); nleft -= n; x -= n;
		n = MIN(nleft, 1 + edge++/2); nleft -= n; y += n;
	}

	printf("%d\n", abs(x) + abs(y));
	return 0;
}
