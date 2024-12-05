#include "common.h"

#define TSZ 100

/* tab[a][b] is -1 if a<b and 1 if a>b */
static int8_t tab[TSZ][TSZ];

static int
cmp(const void *a, const void *b)
{
	return tab[*(const int *)a][*(const int *)b];
}

int
main(int argc, char **argv)
{
	char buf[128], *rest, *tok;
	int p1=0,p2=0, arr[32], n,i, a,b;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while (fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "%d|%d", &a, &b) != 2)
			break;
		assert(a>=0); assert(a<TSZ);
		assert(b>=0); assert(b<TSZ);
		tab[a][b] = -(tab[b][a] = 1);
	}

	while ((rest = fgets(buf, sizeof(buf), stdin))) {
		for (n=0; (tok = strsep(&rest, ",")); n++) {
			assert(n < (int)LEN(arr));
			sscanf(tok, "%d", &arr[n]);
		}

		for (i=1; i<n; i++)
			if (tab[arr[i-1]][arr[i]] > 0 ||
			    tab[arr[i]][arr[i-1]] < 0)
				break;

		if (i==n)
			p1 += arr[n/2];
		else {
			qsort(arr, n, sizeof(*arr), cmp);
			p2 += arr[n/2];
		}
	}

	printf("05: %d %d\n", p1, p2);
	return 0;
}
