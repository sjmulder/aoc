#include "common.h"

int
main(int argc, const char **argv)
{
	char buf[64], *rest, *tok;
	int p1=0, safe,asc=0,cur,prev=0, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		for (safe=1, i=0;
		     safe && (tok = strsep(&rest, " "));
		     i++) {
			cur = (int)strtol(tok, NULL, 10);
			if (i > 0)
				safe = safe && cur != prev &&
				    cur > prev-4 && cur < prev+4;
			if (i == 1)
				asc = cur > prev;
			if (i > 1)
				safe = safe && (cur > prev) == asc;

			prev = cur;
		}

		p1 += safe;
	}

	printf("02: %d\n", p1);
	return 0;
}
