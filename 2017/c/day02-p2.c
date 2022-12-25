#ifdef __clang__
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _WITH_GETLINE
#define LEN(a) (int)(sizeof(a)/sizeof(*a))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(void)
{
	int sum = 0, nums[16], n, val, i, j;
	char *line = NULL, *rest, *tok;
	size_t sz;

	while (getline(&line, &sz, stdin) != -1) {
		rest = line; n = 0;
		while (n < LEN(nums) && (tok = strsep(&rest, " \t")))
			nums[n++] = (int)strtol(tok, NULL, 10);

		val = 0;
		for (i = 0; !val && i < n; i++) {
			for (j = i+1; !val && j < n; j++) {
				if (!(nums[i] % nums[j]))
					val = nums[i] / nums[j];
				if (!(nums[j] % nums[i]))
					val = nums[j] / nums[i];
			}
		}

		sum += val;
		free(line); line = NULL;
	}

	printf("%d\n", sum);
	return 0;
}
