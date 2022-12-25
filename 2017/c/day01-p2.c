#ifdef __clang__
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _WITH_GETLINE

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	char *line = NULL;
	size_t sz;
	int len, sum = 0, i;
	
	if ((len = (int)getline(&line, &sz, stdin)) == EOF) {
		perror(NULL);
		return 1;
	}

	for (i=0; i<len/2; i++)
		if (line[i] == line[len/2 + i])
			sum += line[i]-'0';

	printf("%d\n", sum*2);
	return 0;
}
