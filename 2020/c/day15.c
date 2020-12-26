#include <stdio.h>
#include <assert.h>

#define P1END 2020
#define P2END 30000000

int
main(int argc, char **argv)
{
	static int map[P2END];
	FILE *f;
	int turn, n, prev=0;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	for (turn=0; fscanf(f, "%d,", &n) == 1; turn++) {
		map[prev] = turn;
		prev = n;
	}

	for (; turn < P2END; turn++) {
		n = map[prev] ? turn-map[prev] : 0;
		map[prev] = turn;
		prev = n;

		if (turn == P1END-1)
			printf("%d ", n);
	}

	printf("%d\n", n);
	//getchar();
	return 0;
}
