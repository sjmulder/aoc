#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static int
cmp_u64(const void *a, const void *b)
{
	return *(uint64_t*)a < *(uint64_t*)b ? -1 :
	       *(uint64_t*)a > *(uint64_t*)b ?  1 : 0;
}

int
main()
{
	static char buf[200];
	static uint64_t scores[100];
	int p1=0, nscores=0, depth,i;
	uint64_t score, p2;

bad:	while (fgets(buf, sizeof(buf), stdin)) {
		for (depth=i=0; buf[i] && buf[i] != '\n'; i++)
			switch (buf[i]) {
			case '(':
			case '[':
			case '{':
			case '<':
				buf[depth++] = buf[i];
				break;
			case ')':
				if (!depth || buf[--depth] != '(')
					{ p1 += 3; goto bad; }
				break;
			case ']':
				if (!depth || buf[--depth] != '[')
					{ p1 += 57; goto bad; }
				break;
			case '}':
				if (!depth || buf[--depth] != '{')
					{ p1 += 1197; goto bad; }
				break;
			case '>':
				if (!depth || buf[--depth] != '<')
					{ p1 += 25137; goto bad; }
				break;
			}
		for (score=0; depth>0; depth--)
			switch (buf[depth-1]) {
			case '(': score = score*5 + 1; break;
			case '[': score = score*5 + 2; break;
			case '{': score = score*5 + 3; break;
			case '<': score = score*5 + 4; break;
			}
		scores[nscores++] = score;
	}

	qsort(scores, nscores, sizeof(*scores), cmp_u64);
	p2 = scores[nscores/2];

	printf("10: %d %" PRIu64 "\n", p1, p2);
	return 0;
}
