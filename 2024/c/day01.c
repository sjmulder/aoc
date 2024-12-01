#include "common.h"

static int
cmp_int(const void *a, const void *b)
{
	return *(const int *)a - *(const int *)b;
}

int
main(int argc, const char **argv)
{
	static int a[2][1024];
	int p1=0,p2=0, na=0, c, i,j;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	for (na=0; scanf("%d %d", &a[0][na], &a[1][na]) == 2; na++)
		assert(na+1 < (int)LEN(a[0]));

	qsort(a[0], (size_t)na, sizeof(*a[0]), cmp_int);
	qsort(a[1], (size_t)na, sizeof(*a[1]), cmp_int);

	for (i=0; i<na; i++) {
		for (c=j=0; j<na; j++) c += a[0][i] == a[1][j];

		p1 += abs(a[0][i] - a[1][i]);
		p2 += a[0][i] * c;
	}
	
	printf("01: %d %d\n", p1, p2);	
	return 0;
}
