#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"
#include "../compat/string.h"

#define BUFSZ 1024

int
main(int argc, char **argv)
{
	FILE *f;
	char buf[BUFSZ], *rest, *field;
	int64_t freq, idx;
	int64_t p1now, p1tm, p1bestid=0, p1besttm=LONG_MAX;
	int64_t p2t=0, p2step=1;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	p1now = atoi(fgets(buf, BUFSZ, f));
	rest = fgets(buf, BUFSZ, f);

	for (idx=0; (field = strsep(&rest, ",")); idx++)
		if ((freq = atoi(field))) {
			if ((p1tm = freq - (p1now % freq)) < p1besttm)
				{ p1besttm = p1tm; p1bestid = freq; }
			while ((p2t + idx) % freq)
				p2t += p2step;
			p2step *= freq;
		}

	printf("%" PRId64 " %" PRId64 "\n", p1bestid * p1besttm, p2t);
	//getchar();
	return 0;
}
