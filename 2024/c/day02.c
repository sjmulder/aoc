#include "common.h"

static int
issafe(int *lvs, int n, int skip)
{
	int safe=1, asc=0,prev=0, ns=0,i;

	for (i=0; safe && i<n; i++) {
		if (i == skip)
			{ ns = 1; continue; }
		if (i-ns > 0)
			safe = safe && lvs[i] != prev &&
			    lvs[i] > prev-4 && lvs[i] < prev+4;
		if (i-ns == 1)
			asc = lvs[i] > prev;
		if (i-ns > 1)
			safe = safe && (lvs[i] > prev) == asc;

		prev = lvs[i];
	}

	return safe;
}

int
main(int argc, const char **argv)
{
	char buf[64], *rest, *tok;
	int p1=0,p2=0, lvs[16],n=0, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		for (n=0; (tok = strsep(&rest, " ")); n++) {
			assert(n < (int)LEN(lvs));
			lvs[n] = (int)strtol(tok, NULL, 10);
		}

		for (i=-1; i<n; i++)
			if (issafe(lvs, n, i))
				{ p1 += i == -1; p2++; break; }
	}

	printf("02: %d %d\n", p1, p2);
	return 0;
}
