#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define BUFSZ 1024
#define CAP 128

int
main()
{
	char buf[BUFSZ], *rest, *field;
	int freqs[CAP], nfreqs=0, i;
	long t=0, step=0;

	fgets(buf, BUFSZ, stdin);
	rest = fgets(buf, BUFSZ, stdin);

	while (nfreqs < CAP && (field = strsep(&rest, ",")))
		freqs[nfreqs++] = atoi(field);
	if (nfreqs >= CAP)
		errx(1, "nfreqs overflow");
	
	for (i=0; i<nfreqs; i++)
		if (!freqs[i])
			;
		else if (!step) {
			t = i;
			step = freqs[i];
		} else {
			while ((t+i) % freqs[i])
				t += step;
			step *= freqs[i];
		}

	printf("%ld\n", t);
}
