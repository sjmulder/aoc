#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define BUFSZ 1024

int
main()
{
	char buf[BUFSZ], *rest, *field;
	long freq, idx;
	long p1now, p1tm, p1bestid, p1besttm=LONG_MAX;
	long p2t=0, p2step=1;

	p1now = atoi(fgets(buf, BUFSZ, stdin));
	rest = fgets(buf, BUFSZ, stdin);

	for (idx=0; (field = strsep(&rest, ",")); idx++)
		if ((freq = atoi(field))) {
			if ((p1tm = freq - (p1now % freq)) < p1besttm)
				{ p1besttm = p1tm; p1bestid = freq; }
			while ((p2t + idx) % freq)
				p2t += p2step;
			p2step *= freq;
		}

	printf("%ld %ld\n", p1bestid * p1besttm, p2t);
}
