#pragma clang diagnostic ignored "-Wreserved-id-macro"

#define _WITH_GETLINE
#define LEN(a) (sizeof(a)/sizeof(*a))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int
compar_char(const void *a, const void *b)
{
	return *(const char*)b - *(const char*)a;
}

int
main(void)
{
	int nvalid=0;
	char *line=NULL, *rest, *words[32];
	size_t sz, i, j;
	ssize_t len;

	while ((len = getline(&line, &sz, stdin)) != -1) {
		rest = line;
		if (line[len-1] == '\n')
			line[len-1] = '\0';
		for (i = 0; i < LEN(words); i++) {
			if (!(words[i] = strsep(&rest, " ")))
				break;
			qsort(words[i], strlen(words[i]), 1, compar_char);
			for (j = 0; j < i; j++)
				if (!strcmp(words[i], words[j]))
					goto end;
		}
		nvalid++;
	end:	free(line);
		line = NULL;
	}

	printf("%d\n", nvalid);
	return 0;
}
