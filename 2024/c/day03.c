#include "common.h"

static int
parse_mul(const char **stringp, int *ap, int *bp)
{
	const char *cur = *stringp, *end;

	if (strncmp(cur, "mul(", 4)) { return 0; } cur += 4;
	*ap = (int)strtol(cur, (char **)&end, 10);
	if (end == cur)  { return 0; } cur = end;
	if (*cur != ',') { return 0; } cur += 1;
	*bp = (int)strtol(cur, (char **)&end, 10);
	if (end == cur)  { return 0; } cur = end;
	if (*cur != ')') { return 0; } cur += 1;

	*stringp = cur;
	return 1;
}

int
main(int argc, char **argv)
{
	static char buf[32*1024];
	const char *p;
	size_t nr;
	int p1=0,p2=0, a,b, dont=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	nr = fread(buf, 1, sizeof(buf), stdin);
	assert(!ferror(stdin));
	assert(nr != sizeof(buf));
	buf[nr] = '\0';

	for (p = buf; *p; )
		if (parse_mul(&p, &a, &b)) { p1 += a*b; p2 += a*b*!dont; }
		else if (!strncmp(p, "do()", 4))    { dont = 0; p += 4; }
		else if (!strncmp(p, "don't()", 7)) { dont = 1; p += 7; }
		else p++;

	printf("03: %d %d\n", p1, p2);
}
