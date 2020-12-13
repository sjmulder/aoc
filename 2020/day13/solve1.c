#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZ 1024

int
main()
{
	char buf[BUFSZ], *rest, *field;
	int now, id,min, bestmin=9999,bestid=0;

	now = atoi(fgets(buf, BUFSZ, stdin));
	rest = fgets(buf, BUFSZ, stdin);

	while ((field = strsep(&rest, ",")))
		if (field[0] != 'x') {
			id = atoi(field);
			min = id - (now % id);
			if (min < bestmin)
				{ bestmin = min; bestid = id; }
		}

	printf("%d\n", bestid * bestmin);
}
