#include <stdio.h>
#include <string.h>
#include <assert.h>

void p1_set(int x, int y, char val);
void p2_set(int x, int y, char val);

int p1_run(void);
int p2_run(void);

int
main(int argc, char **argv)
{
	FILE *f;
	int x=0,y=0, c;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	for (; (c = fgetc(f)) != EOF; x++)
		if (c == '#') {
			p1_set(x, y, 1);
			p2_set(x, y, 1);
		} else if (c == '\n')
			{ x = -1; y++; }

	printf("%d ",  p1_run());
	printf("%d\n", p2_run());
	//getchar();
	return 0;
}
