#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>

#define NJOBS 4
#define MAXLINES 100000
#define NOSOLVE 2

static int work(int);
static int sndiff(char *, char *);
static void pcommon(char *, char *);

static char lines[MAXLINES][32];
static size_t n;

int
main()
{
	int job;
	int status;

	while (n < sizeof(lines) / sizeof(*lines)) {
		if (fgets(lines[n], sizeof(*lines), stdin))
			n++;
		else if (ferror(stdin))
			err(1, "<stdin>");
		else
			break;
	}

	for (job = 0; job < NJOBS; job++)
		if (fork() == 0)
			return work(job);

	while (wait(&status) != -1)
		if (status == 0) /* solved! */
			return 0;

	puts("no solution");
	return NOSOLVE;
}

/* returns 0 on success, NOSOLVE on error */
static int
work(int job)
{
	size_t i, j;

	for (i = job; i < n; i += NJOBS)
		for (j = i+1; j < n; j++)
			if (sndiff(lines[i], lines[j]) == 1) {
				pcommon(lines[i], lines[j]);
				return 0;
			}

	return NOSOLVE;
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
