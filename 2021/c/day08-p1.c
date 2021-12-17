#include <stdio.h>
#include <string.h>
#include "compat.h"

int
main()
{
	char line[100], *rest,*lf,*word;
	int p1=0, len;

	while ((rest = fgets(line, 100, stdin))) {
		if ((lf = strchr(rest, '\n')))
			*lf = '\0';
		while ((word = strsep(&rest, " ")) && *word != '|')
			;
		while ((word = strsep(&rest, " "))) {
			len = (int)strlen(word);
			p1 += len != 5 && len != 6;
		}
	}

	printf("08: %d\n", p1);
}
