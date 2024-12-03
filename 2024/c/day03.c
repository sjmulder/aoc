#include "common.h"

static int
parse_exact(const char **stringp, const char *expect)
{
	const char *s = *stringp;
	int i;

	for (i=0; s[i] && expect[i] && s[i] == expect[i]; i++)
		;
	if (expect[i])
		return 0;

	*stringp  = &s[i];
	return 1;
}

static int
parse_int(const char **stringp, int *outp)
{
	char *end;
	int val;

	val = (int)strtol(*stringp, &end, 10);
	if (end == *stringp)
		return 0;

	*stringp = end;
	if (outp) *outp = val;
	return 1;
}

static int
parse_mul(const char **stringp, int *ap, int *bp)
{
	const char *cur = *stringp;
	int a,b;

	if (!parse_exact(&cur, "mul(") ||
	    !parse_int(&cur, &a) ||
	    !parse_exact(&cur, ",") ||
	    !parse_int(&cur, &b) ||
	    !parse_exact(&cur, ")"))
		return 0;

	*stringp = cur;
	if (ap) *ap = a;
	if (bp) *bp = b;
	return 1;
}

int
main(int argc, char **argv)
{
	static char buf[32*1024];
	const char *cur;
	size_t nr;
	int p1=0,p2=0, a,b, dont=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	nr = fread(buf, 1, sizeof(buf), stdin);
	assert(!ferror(stdin));
	assert(nr != sizeof(buf));
	buf[nr] = '\0';

	for (cur = buf; *cur; )
		if (parse_exact(&cur, "do()"))
			dont = 0;
		else if (parse_exact(&cur, "don't()"))
			dont = 1;
		else if (parse_mul(&cur, &a, &b)) {
			p1 += a * b;
			if (!dont) p2 += a * b;
		} else
			cur++;

	printf("03: %d %d\n", p1, p2);
}
