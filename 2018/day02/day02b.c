#include <stdio.h>
#include <string.h>
#include <err.h>

static int sndiff(char *, char *);
static void pcommon(char *, char *);

int
main()
{
	char lines[100000][32];
	size_t n = 0;
	size_t i, j;

	while (n < sizeof(lines) / sizeof(*lines)) {
		if (fgets(lines[n], sizeof(*lines), stdin))
			n++;
		else if (ferror(stdin))
			err(1, "<stdin>");
		else
			break;
	}

	for (i = 0; i < n; i++) {
		for (j = i+1; j < n; j++) {
			if (sndiff(lines[i], lines[j]) == 1) {
				pcommon(lines[i], lines[j]);
				return 0;
			}
		}
	}

	puts("no solution");
	return 0;
}

static int
sndiff(char *s1, char *s2)
{
	int ndiff = 0;

	while (*s1 && *s2 && ndiff < 2)
		if (*(s1++) != *(s2++))
			ndiff++;
	
	return ndiff + strlen(s1) + strlen(s2);
}

static void
pcommon(char *s1, char *s2)
{
	while (*s1 && *s2)
		if (*(s1++) == *(s2++))
			putchar(s1[-1]);
}
