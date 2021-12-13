#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

static int pts[1000][2], npt;

static int
cmp_pt(const void *va, const void *vb)
{
	const int *a=va, *b=vb;

	return a[1]<b[1]? -1 : a[1]>b[1] ? 1 : a[0]-b[0];
}

static void
uniq(void)
{
	int nleft=1, i;

	for (i=1; i<npt; i++)
		if (memcmp(&pts[i], &pts[i-1], sizeof(*pts)))
			memmove(&pts[nleft++], &pts[i], sizeof(*pts));

	npt = nleft;
}

static void
dump(void)
{
	int i;

	for (i=0; i<npt; i++)
		printf("%d,%d\n", pts[i][0], pts[i][1]);

	printf("  n=%d\n", npt);
}

int
main()
{
	char buf[20], axis;
	int dim,n,i,x,y, p1=0, xmax=0,ymax=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		assert(npt < (int)sizeof(pts));
		if (sscanf(buf, "%d,%d", pts[npt], pts[npt]+1) != 2)
			break;
		npt++;
	}

	//qsort(pts, npt, sizeof(*pts), cmp_pt);
	//dump();

	while (scanf(" fold along %c=%d", &axis, &n) == 2) {
		//printf("folding along %c=%d\n", axis, n);

		dim = axis=='y';

		for (i=0; i<npt; i++)
			if (pts[i][dim] > n)
				pts[i][dim] = n - (pts[i][dim]-n);

		qsort(pts, npt, sizeof(*pts), cmp_pt);
		uniq();
		//dump();

		if (!p1)
			p1 = npt;
	}

	printf("13 (p1): %d\n", p1);

	for (i=0; i<npt; i++) {
		if (xmax < pts[i][0]) xmax = pts[i][0];
		if (ymax < pts[i][1]) ymax = pts[i][1];
	}

	//dump();

	i=0;
	for (y=0; y<=ymax; y++) {
		printf("13 (p2): ");
		for (x=0; x<=xmax; x++)
			if (i<npt && pts[i][0]==x && pts[i][1]==y)
				{ putchar('#'); i++; }
			else
				putchar(' ');
		putchar('\n');
	}

	return 0;
}
