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
compare_lists(char *a, char *b)
{
	struct tok a_tok, b_tok;
	char buf[16];

	a_tok = parse_tok(a, &a);
	b_tok = parse_tok(b, &b);

	switch (a_tok.type) {
	case TOK_INT:
		switch (b_tok.type) {
		case TOK_INT:
			if (a_tok.val < b_tok.val) return -1;
			if (a_tok.val > b_tok.val) return  1;
			return compare_lists(a, b);
		case TOK_LIST_BEGIN:
			snprintf(buf, sizeof(buf), "%d]", a_tok.val);
			return compare_lists(buf, b);
		case TOK_LIST_END:
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_LIST_BEGIN:
		switch (b_tok.type) {
		case TOK_INT:
			snprintf(buf, sizeof(buf), "%d]", b_tok.val);
			return compare_lists(a, buf);
		case TOK_LIST_BEGIN:
			return compare_lists(a, b);
		case TOK_LIST_END:
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_LIST_END:
		switch (b_tok.type) {
		case TOK_INT:
		case TOK_LIST_BEGIN:
			return -1;
		case TOK_LIST_END:
			return compare_lists(a, b);
		case TOK_EOF:
		default:
			return 1;
		}
	case TOK_EOF:
	default:
		switch (b_tok.type) {
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
	static char line1[256], line2[256];
	int p1=0, dec1=0, dec2=0, i;

	for (i=0; ; i++) {
		if (!(fgets(line1, sizeof(line1), stdin))) break;
		if (!(fgets(line2, sizeof(line2), stdin))) break;

		if (compare_lists(line1, line2) < 0)
			p1 += i+1;

		dec1 += compare_lists(line1, "[[2]]") < 0;
		dec1 += compare_lists(line2, "[[2]]") < 0;
		dec2 += compare_lists(line1, "[[6]]") < 0;
		dec2 += compare_lists(line2, "[[6]]") < 0;

		fgets(line1, sizeof(line1), stdin);
	}

	printf("13: %d %d\n", p1,
	    (dec1 + (dec1>dec2) +1) *
	    (dec2 + (dec2>dec1) +1));

	return 0;
}
