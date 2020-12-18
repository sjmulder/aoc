#include <stdio.h>
#include <stdlib.h>
#include <err.h>

static long eval(char **, int);

static long
nextval(char **sp, int pt)
{
	long val;
	char *end;

	while (1)
		switch (**sp) {
		case ' ': (*sp)++; continue;
		case '(':
			(*sp)++;
			val = eval(sp, pt);
			if (**sp != ')')
				errx(1, "expected ): %c\n", **sp);
			(*sp)++;
			return val;
		default:
			val = strtol(*sp, &end, 10);
			if (*sp == end)
				errx(1, "expected number: %s\n", *sp);
			*sp = end;
			return val;
		}
}

static long
eval(char **sp, int pt)
{
	long acc;

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
		default:   errx(1, "unexpected: %c\n", **sp);
		}
}

int
main()
{
	long p1=0, p2=0;
	char *s, buf[512];

	while (fgets(buf, sizeof(buf), stdin)) {
		s = buf; p1 += eval(&s, 1);
		s = buf; p2 += eval(&s, 2);
	}

	printf("%ld %ld\n", p1, p2);
}
