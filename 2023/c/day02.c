#include "common.h"

int
main(int argc, char **argv)
{
	char line[256], *sr,*sid,*srd,*s;
	int p1=0,p2=0, id, r,g,b;

	if (argc > 1)
	       DISCARD(freopen(argv[1], "r", stdin));

	while ((sr = fgets(line, sizeof(line), stdin))) {
		sid = strsep(&sr, ":");
		sscanf(sid, "Game %d", &id);
		r = g = b = 0;

		while ((srd = strsep(&sr, ";")))
		while ((s = strsep(&srd, ",")))
			if (strchr(s, 'd')) r = MAX(r, atoi(s)); else
			if (strchr(s, 'g')) g = MAX(g, atoi(s)); else
			if (strchr(s, 'b')) b = MAX(b, atoi(s));

		p1 += (r <= 12 && g <= 13 && b <= 14) * id;
		p2 += r * g * b;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
