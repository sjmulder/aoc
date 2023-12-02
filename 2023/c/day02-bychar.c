#include "common.h"

int
main(int argc, char **argv)
{
	int p1=0,p2=0, id=1,num=0, r=0,g=0,b=0, c;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while ((c = getchar()) != EOF)
		if (c==',' || c==';' || c==':') num = 0; else
		if (c>='0' && c<='9') num = num*10 + c-'0'; else
		if (c=='d') r = MAX(r, num); else
		if (c=='g') g = MAX(g, num); else
		if (c=='b') b = MAX(b, num); else
		if (c=='\n') {
			p1 += (r<=12 && g<=13 && b<=14) * id;
			p2 += r*g*b;
			r=g=b=0; id++;

		}

	printf("%d %d\n", p1, p2);
	return 0;
}
