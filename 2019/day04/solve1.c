#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <err.h>

static void increment(char *, size_t);
static int validate(char *);

int
main(int argc, char **argv)
{
	char *num, *to;
	size_t len;
	int nvalid = 0, is_last;

	if (argc != 3)
		errx(1, "usage: %s <from> <to>", argv[0]);

	num = argv[1];
	to = argv[2];
	len = strlen(num);

	assert(len == strlen(to));

	while (1) {
		is_last = !strcmp(num, to);
		if (validate(num))
			nvalid++;
		if (is_last)
			break;
		increment(num, len);
	}

	printf("%d\n", nvalid);
	return 0;
}

static void
increment(char *num, size_t len)
{
	size_t digit = len-1;

	while (num[digit] == '9') {
		assert(digit);
		num[digit--] = '0';
	}

	num[digit]++;
}

static int
validate(char *num)
{
	int ndouble = 0;

	assert(num[0]);

	for(; num[1]; num++) {
		if (num[0] > num[1])
			return 0;
		if (num[0] == num[1])
			ndouble++;
	}

	return ndouble > 0;
}
