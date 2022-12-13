#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

enum { TOK_INT, TOK_LIST_BEGIN, TOK_LIST_END, TOK_EOF };
struct tok { int type, val; };

static struct tok
parse_tok(char *s, char **endp)
{
	struct tok ret;

	assert(s);
	memset(&ret, 0, sizeof(ret));

	if (*s == ',')
		s++;

	if (*s == '[') {
		ret.type = TOK_LIST_BEGIN;
		if (endp) *endp = s+1;
	} else if (*s == ']') {
		ret.type = TOK_LIST_END;
		if (endp) *endp = s+1;
	} else if (isdigit(*s)) {
		ret.type = TOK_INT;
		ret.val = (int)strtol(s, endp, 10);
	} else
		ret.type = TOK_EOF;

	return ret;
}

static int
compare_lists(char *ls, char *rs)
{
	struct tok lt, rt;
	char buf[16];

	lt = parse_tok(ls, &ls);
	rt = parse_tok(rs, &rs);

	switch (lt.type) {
	case TOK_INT:
		switch (rt.type) {
		case TOK_INT:
			if (lt.val < rt.val) return -1;
			if (lt.val > rt.val) return  1;
			return compare_lists(ls, rs);
		case TOK_LIST_BEGIN:
			snprintf(buf, sizeof(buf), "%d]",
			    lt.val);
			return compare_lists(buf, rs);
		case TOK_LIST_END:
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_LIST_BEGIN:
		switch (rt.type) {
		case TOK_INT:
			snprintf(buf, sizeof(buf), "%d]",
			    rt.val);
			return compare_lists(ls, buf);
		case TOK_LIST_BEGIN:
			return compare_lists(ls, rs);
		case TOK_LIST_END:
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_LIST_END:
		switch (rt.type) {
		case TOK_INT:
		case TOK_LIST_BEGIN:
			return -1;
		case TOK_LIST_END:
			return compare_lists(ls, rs);
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_EOF:
	default:
		switch (rt.type) {
		case TOK_INT:
		case TOK_LIST_BEGIN:
		case TOK_LIST_END:
			return -1;
		case TOK_EOF:
		default:
			return 0;
		}
	}
}

int
main()
{
	static char buf1[256], buf2[256];
	int p1=0, dec1=0, dec2=0, i;

	for (i=0; ; i++) {
		if (!(fgets(buf1, sizeof(buf1), stdin))) break;
		if (!(fgets(buf2, sizeof(buf2), stdin))) break;

		if (compare_lists(buf1, buf2) < 0)
			p1 += i+1;

		dec1 += compare_lists(buf1, "[[2]]") < 0;
		dec1 += compare_lists(buf2, "[[2]]") < 0;
		dec2 += compare_lists(buf1, "[[6]]") < 0;
		dec2 += compare_lists(buf2, "[[6]]") < 0;

		fgets(buf1, sizeof(buf1), stdin);
	}

	printf("13: %d %d\n", p1,
	    (dec1 + (dec1>dec2) +1) *
	    (dec2 + (dec2>dec1) +1));

	return 0;
}
