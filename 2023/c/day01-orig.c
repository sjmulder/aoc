#include "common.h"

int
main(int argc, char **argv)
{
	static const char * const nm[] = {"zero", "one", "two", "three",
	    "four", "five", "six", "seven", "eight", "nine"};
	char buf[64], *s;
	int p1=0,p2=0, p1f,p1l, p2f,p2l, d;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (fgets(buf, sizeof(buf), stdin)) {
		p1f = p1l = p2f = p2l = -1;

		for (s=buf; *s; s++)
			if (*s >= '0' && *s <= '9') {
				d = *s-'0';
				if (p1f == -1) p1f = d;
				if (p2f == -1) p2f = d;
				p1l = p2l = d;
			} else for (d=0; d<10; d++) {
				if (strncmp(s, nm[d], strlen(nm[d])))
					continue;
				if (p2f == -1) p2f = d;
				p2l = d;
				break;
			}

		p1 += p1f*10 + p1l;
		p2 += p2f*10 + p2l;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
