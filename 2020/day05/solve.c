#include <stdio.h>
#include <assert.h>

int
main(int argc, char **argv)
{
	static char map[128*8];
	FILE *f;
	int c,i, n=0, max=0;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while ((c = fgetc(f)) != EOF)
		if (c=='\n') { map[n] = 1; if (n>max) max=n; n = 0; }
		else         { n = n<<1 | (c=='B') | (c=='R'); }

	for (i=1; i < 128*8-1; i++)
		if (map[i-1] && !map[i] && map[i+1])
			{ printf("%d %d\n", max, i); break; }

	getchar();
	return 0;
}
