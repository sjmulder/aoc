#include <stdio.h>
#include <err.h>

int ccount(char *, char);

int
main()
{
	char line[32]; 
	char c;
	int have2, n2 = 0;
	int have3, n3 = 0;
	int cm;

	while (fgets(line, sizeof(line), stdin)) {
		have2 = 0;
		have3 = 0;
		for (c = 'a'; c <= 'z'; c++) {
			cm = ccount(line, c);
			if (cm >= 3)
				have3 = 1;
			else if (cm == 2)
				have2 = 1;
		}
		if (have2)
			n2++;
		if (have3)
			n3++;
	}

	if (ferror(stdin))
		err(1, "fgets");
	
	printf("%d\n", n2 * n3);
	return 0;
}

int
ccount(char *s, char c)
{
	int count = 0;

	while (*s)
		if (*(s++) == c)
			count++;

	return count;
}
