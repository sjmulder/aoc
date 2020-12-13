#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZ 1024

int
main()
{
	char buf[BUFSZ], *rest, *field;
	long t=0, step=1, off, freq;

	fgets(buf, BUFSZ, stdin);
	rest = fgets(buf, BUFSZ, stdin);

	for (off=0; (field = strsep(&rest, ",")); off++)
		if ((freq = atoi(field))) {
			while ((t+off) % freq)
				t += step;
			step *= freq;
		}

	printf("%ld\n", t);
}
