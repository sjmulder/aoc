#include <stdio.h>
#include <stdlib.h>

#define P1END 2020
#define P2END 30000000

int
main()
{
	static int log[P2END];
	int turn=0, i;

	for (turn=0; ; turn++)
		if (scanf("%d,\n", &log[turn]) != 1)
			break;

	for (; turn < P2END; turn++) {
		for (i = turn-2; i>=0; i--)
			if (log[i] == log[turn-1])
				break;
		log[turn] = i<0 ? 0 : turn-i-1;
		if (turn == P1END-1)
			printf("%d\n", log[turn]);
		if (!(turn % 10000))
			printf(" %dk\n", turn / 1000);
	}

	printf("%d\n", log[P2END-1]);
}

