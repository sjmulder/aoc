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
	long ids[CAP],offs[CAP], nids=0,off=0,id, t0=0,step=1, t,i;

	fgets(buf, BUFSZ, stdin);
	rest = fgets(buf, BUFSZ, stdin);

	while ((field = strsep(&rest, ","))) {
		if (nids >= CAP)
			errx(1, "ids overflow");
		if ((id = atol(field))) {
			ids[nids] = id;
			offs[nids] = off;
			nids++;
		}
		if (id > step) {
			step = id;
			t0 = id-off;
		}
		off++;
	}

	for (t=t0; ; t += step) {
		for (i=0; i<nids; i++)
			if ((t+offs[i]) % ids[i])
				break;
		if (i == nids)
			break;
	}

	printf("%ld\n", t);
}
