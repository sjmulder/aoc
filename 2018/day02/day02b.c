#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NJOBS 4
#define DATASZ (4 * 1024 * 1024)
#define NOSOLVE 2

#define LINE(i) (data + (i)*(linelen+1))

static int work(int);
static int ndiff(char *, char *);
static void pcommon(char *, char *);

static char *data;
static size_t linelen;
static size_t nlines;

int
main()
{
	size_t nread;
	int job;
	int status;

	data = malloc(DATASZ);
	nread = fread(data, 1, DATASZ, stdin);
	linelen = strchr(data, '\n') - data;
	nlines = nread / linelen;

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
	size_t a, b;

	for (a = job; a < nlines; a += NJOBS)
		for (b = a+1; b < nlines; b++)
			if (ndiff(LINE(a), LINE(b)) == 1) {
				pcommon(LINE(a), LINE(b));
				return 0;
			}

	return NOSOLVE;
}

/* number of different characters in a line, exits after 2 */
static int
ndiff(char *s1, char *s2)
{
	size_t i;
	int ndiff = 0;

	for (i = 0; i < linelen && ndiff < 2; i++)
		if (s1[i] != s2[i])
			ndiff++;
	return ndiff;
}

static void
pcommon(char *s1, char *s2)
{
	size_t i;

	for (i = 0; i < linelen; i++)
		if (s1[i] == s2[i])
			putchar(s1[i]);
	putchar('\n');
}
