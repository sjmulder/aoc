#ifdef __clang__
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _WITH_GETLINE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int
main(void)
{
	int sum = 0, min, max, n;
	char *line = NULL, *rest, *tok;
	size_t sz;

	while (getline(&line, &sz, stdin) != -1) {
		rest = line; min = INT_MAX; max = INT_MIN;
		while ((tok = strsep(&rest, " \t"))) {
			n = (int)strtol(tok, NULL, 10);
			if (n < min) min = n;
			if (n > max) max = n;
		}

		sum += max - min;
		free(line); line = NULL;
	}

	printf("%d\n", sum);
	return 0;
}
