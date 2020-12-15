#include <stdio.h>

#define P1END 2020
#define P2END 30000000

int
main()
{
	static int map[P2END];
	int turn, n, prev=0;

	for (turn=0; scanf("%d,", &n) == 1; turn++) {
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
}

