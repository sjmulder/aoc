#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

static int64_t eval(char **, int);

static int64_t
nextval(char **sp, int pt)
{
	int64_t val;
	char *end;

	while (1)
		switch (**sp) {
		case ' ': (*sp)++; continue;
		case '(':
			(*sp)++;
			val = eval(sp, pt);
			assert(**sp == ')');
			(*sp)++;
			return val;
		default:
			val = strtol(*sp, &end, 10);
			assert(*sp != end);
			*sp = end;
			return val;
		}
}

static int64_t
eval(char **sp, int pt)
{
	int64_t acc;

	acc = nextval(sp, pt);

	while (1)
		switch (**sp) {
		case '\0': return acc;
		case '\n': return acc;
		case ')':  return acc;
		case ' ':  (*sp)++; continue;
		case '+':  (*sp)++; acc += nextval(sp,pt); break;
		case '*':
			(*sp)++;
			acc *= pt==1 ? nextval(sp,pt) : eval(sp,pt);
			break;
		default:   assert(0 && "unexpected char");
		}
}

int
main(int argc, char **argv)
{
	FILE *f;
	int64_t p1=0, p2=0;
	char *s, buf[512];

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while (fgets(buf, sizeof(buf), f)) {
		s = buf; p1 += eval(&s, 1);
		s = buf; p2 += eval(&s, 2);
	}

	printf("%" PRId64 " %" PRId64 "\n", p1, p2);
	getchar();
	return 0;
}
