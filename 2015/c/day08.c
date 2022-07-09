#include <stdio.h>
#include <string.h>

int
main()
{
	char s[64], *lf;
	size_t len, i;
	int p1=0, p2=0;

	while (fgets(s, sizeof(s), stdin)) {
		if ((lf = strchr(s, '\n')))
			*lf = '\0';
		len = strlen(s);

		for (i=0; i<len; )
			if (s[i] == '\\' && s[i+1] == 'x')
				{ p1 += 3; i += 4; }
			else if (s[i] == '\\')
				{ p1 += 1; i += 2; }
			else if (s[i] == '"')
				{ p1 += 1; i += 1; }
			else
				i += 1;

		p2 += 2;
		for (i=0; i<len; i++)
			p2 += s[i] == '"' || s[i] == '\\';
	}

	printf("08: %d %d\n", p1, p2);
	return 0;
}
