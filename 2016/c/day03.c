#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _WITH_GETLINE

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	char *line = NULL;
	size_t sz;
	unsigned long a, b, c, ab, ac, bc, n = 0;

	while (getline(&line, &sz, stdin) && *line) {
		if (sscanf(line, " %lu %lu %lu", &a, &b, &c) != 3) {
			fprintf(stderr, "invalid input: '%s'\n", line);
			return 1;
		}

		if (!__builtin_add_overflow(a, b, &ab)
				&& !__builtin_add_overflow(a, c, &ac)
				&& !__builtin_add_overflow(b, c, &bc)
				&& a < bc && b < ac && c < ab)
			n++;

		free(line);
		line = NULL;
	}

	printf("%lu\n", n);
	return 0;
}
