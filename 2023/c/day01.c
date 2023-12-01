#include "common.h"

static const char * const names[] = {
	"zero", "one", "two", "three", "four",
	"five", "six", "seven", "eight", "nine"
};

static int
parse_digit(const char *s, int part)
{
	int i;

	if (*s >= '0' && *s <= '9')
		return *s-'0';

	if (part)
		for (i=0; i<10; i++)
			if (!strncmp(s, names[i], strlen(names[i])))
				return i;
	
	return -1;
}

static int
get_value(const char *s, int part)
{
	int digit, first=-1, last=-1;

	for (; *s; s++)
		if ((digit = parse_digit(s, part)) != -1) {
			if (first == -1) first = digit;
			last = digit;
		}

	return first*10 + last;
}

int
main(int argc, char **argv)
{
	char buf[64];
	int p1=0, p2=0;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (fgets(buf, sizeof(buf), stdin)) {
		p1 += get_value(buf, 0);
		p2 += get_value(buf, 1);
	}

	printf("%d %d\n", p1, p2);

	return 0;
}
