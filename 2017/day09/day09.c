#include <stdio.h>

static int score, ngarbage, depth;

static void
readgarbage()
{
	while (!feof(stdin)) {
		switch (getchar()) {
			case '!': getchar(); break;
			case '>': return;
			default: ngarbage++; break;
		}
	}
}

static void
readgroup()
{
	score += depth++;

	while (!feof(stdin)) {
		switch (getchar()) {
			case '{': readgroup(); break;
			case '<': readgarbage(); break;
			case '}': depth--; return;
		}
	}
}

int
main()
{
	readgroup();
	printf("score:   %d\n", score);
	printf("garbage: %d\n", ngarbage);
	return 0;
}
