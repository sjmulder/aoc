#include "common.h"

static int64_t
count_configs(const char *chars, int nchars, int *runs, int nruns)
{
	static int depth=0;
	static int64_t memoi[128][40];
	int nrest, off, i;
	int64_t n=0;

	if (nchars <= 0)
		return nruns == 0;
	if (nruns <= 0)
		return !memchr(chars, '#', nchars);

	assert(nchars < (int)LEN(memoi));
	assert(nruns < (int)LEN(*memoi));

	if (!depth)
		memset(memoi, 0, sizeof(memoi));
	else if (memoi[nchars][nruns])
		return memoi[nchars][nruns]-1;

	depth++;

	if (nruns == 1)
		nrest = 0;
	else for (nrest = nruns-2, i=1; i<nruns; i++)
		nrest += runs[i];

	for (off=0; off+runs[0]+nrest <= nchars; off++) {
		if (chars[off+runs[0]] == '#' ||	/* overruns */
		    memchr(chars, '#', off) ||		/* # in run */
		    memchr(chars+off, '.', runs[0]))	/* . in run */
			continue;
		n += count_configs(
		    chars+off+runs[0]+1,
		    nchars-off-runs[0]-1,
		    runs+1, nruns-1);
	}

	depth--;
	memoi[nchars][nruns] = n+1;

	return n;
}

#if WITH_TEST
static void
test(void)
{
	int runs[] = {3, 1, 2};

	assert(count_configs("??",      2, runs, 1) == 0);
	assert(count_configs("???",     3, runs, 1) == 1);
	assert(count_configs("?????",   5, runs, 1) == 3);
	assert(count_configs("????",    4, runs, 2) == 0);
	assert(count_configs("?????",   5, runs, 2) == 1);
	assert(count_configs("###.#..", 7, runs, 2) == 1);
	assert(count_configs("?##?.#.", 7, runs, 2) == 2);
	assert(count_configs("???????", 7, runs, 2) == 6);

	assert(count_configs("???..", 5, runs, 1) == 1);
	assert(count_configs(".???.", 5, runs, 1) == 1);
	assert(count_configs("..???", 5, runs, 1) == 1);

	assert(count_configs("#????", 5, runs, 1) == 1);
	assert(count_configs("?#???", 5, runs, 1) == 2);
	assert(count_configs("??#??", 5, runs, 1) == 3);
	assert(count_configs("???#?", 5, runs, 1) == 2);
	assert(count_configs("????#", 5, runs, 1) == 1);
}
#endif

int
main(int argc, char **argv)
{
	char line[64], chars2[128], *rest, *tok, *chars;
	int runs[40], nruns, i,j;
	int64_t p1=0,p2=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

#ifdef WITH_TEST
	test();
#endif

	while ((rest = fgets(line, sizeof(line), stdin))) {
		chars = strsep(&rest, " ");
		snprintf(chars2, sizeof(chars2), "%s?%s?%s?%s?%s",
		    chars, chars, chars, chars, chars);

		for (nruns=0; (tok = strsep(&rest, ",")); ) {
			assert(nruns < (int)LEN(runs) /5);
			runs[nruns++] = atoi(tok);
		}

		for (i=1; i<5; i++)
		for (j=0; j<nruns; j++)
			runs[i*nruns+j] = runs[j];

		p1 += count_configs(chars, (int)strlen(chars), runs,
		    nruns);
		p2 += count_configs(chars2, (int)strlen(chars2), runs,
		    nruns*5);
	}

	printf("12: %"PRIi64" %"PRIi64"\n", p1, p2);
	return 0;
}
