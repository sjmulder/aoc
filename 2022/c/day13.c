#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "compat/stdio.h"

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

enum { TOK_INT, TOK_LIST_BEGIN, TOK_LIST_END, TOK_EOF };
struct tok { int type, val; };

static int sgn(int x) { return x<0 ? -1 : x>0 ? 1 : 0; }

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
	char buf[256];

tail_recurse:
	a_tok = parse_tok(a, &a);
	b_tok = parse_tok(b, &b);

	if (a_tok.type == TOK_EOF)
		return -1;
	if (a_tok.type == b_tok.type && a_tok.val  == b_tok.val)
		goto tail_recurse;
	if (a_tok.type == TOK_INT && b_tok.type == TOK_INT)
		return sgn(a_tok.val - b_tok.val);
	if (a_tok.type == TOK_INT && b_tok.type == TOK_LIST_BEGIN) {
		snprintf(buf, sizeof(buf), "%d]%s", a_tok.val, a);
		return compare_lists(buf, b);
	}
	if (a_tok.type == TOK_LIST_BEGIN && b_tok.type == TOK_INT) {
		snprintf(buf, sizeof(buf), "%d]%s", b_tok.val, b);
		return compare_lists(a, buf);
	}

	return sgn(b_tok.type - a_tok.type);
}

int
main(int argc, char **argv)
{
	static char line1[256], line2[256];
	int p1=0, dec1=1, dec2=2, i;

	if (argc > 1)
		(void)!freopen(argv[1], "r", stdin);

	for (i=0; ; i++) {
		if (!(fgets(line1, sizeof(line1), stdin))) break;
		if (!(fgets(line2, sizeof(line2), stdin))) break;

		if (compare_lists(line1, line2) < 0)
			p1 += i+1;

		dec1 += compare_lists(line1, "[[2]]") < 0;
		dec1 += compare_lists(line2, "[[2]]") < 0;
		dec2 += compare_lists(line1, "[[6]]") < 0;
		dec2 += compare_lists(line2, "[[6]]") < 0;

		(void)!fgets(line1, sizeof(line1), stdin);
	}

	printf("13: %d %d\n", p1, dec1 * dec2);
	return 0;
}
