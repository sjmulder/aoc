#include <stdio.h>
#include <stdlib.h>

const char fmt[] =
"%*s can fly %d km/s for %d seconds, but then must rest for %d "
"seconds.";

int
main(int argc, char **argv)
{
	int t,d, tend=2503, best=0, kms, fly, rest;

	if (argc > 1)
		tend = atoi(argv[1]);

	while (scanf(fmt, &kms, &fly, &rest) == 3) {
		d=0; t=tend;

		while (t>fly)
			{ d += kms*fly; t -= fly+rest; }
		if (t>0)
			d += t * kms;
		if (d>best)
			best = d;
	}

	printf("%d\n", best);
	return 0;
}
