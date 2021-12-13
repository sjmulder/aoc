#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

#define X 0
#define Y 1

static int pts[100000][2], npt;

static int
cmp_pt(const void *va, const void *vb)
{
	const int *a=va, *b=vb;

	return a[Y]<b[Y]? -1 : a[Y]>b[Y] ? 1 : a[X]-b[X];
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

int
main()
{
	char buf[20], axis;
	int dim,n,i,x,y, p1=0, xmax=0,ymax=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		assert(npt < (int)sizeof(pts));
		if (sscanf(buf, "%d,%d", &pts[npt][X], &pts[npt][Y])!=2)
			break;
		npt++;
	}

	while (scanf(" fold along %c=%d", &axis, &n) == 2) {
		dim = axis=='x' ? X : Y;
		for (i=0; i<npt; i++)
			pts[i][dim] = n - abs(n-pts[i][dim]);

		qsort(pts, npt, sizeof(*pts), cmp_pt);
		uniq();

		if (!p1)
			p1 = npt;
	}

	printf("13 (p1): %d\n", p1);

	for (i=0; i<npt; i++) {
		if (xmax < pts[i][X]) xmax = pts[i][X];
		if (ymax < pts[i][Y]) ymax = pts[i][Y];
	}

	for (i=0, y=0; y<=ymax; y++) {
		printf("13 (p2): ");
		for (x=0; x<=xmax; x++)
			if (i<npt && pts[i][X]==x && pts[i][Y]==y)
				{ putchar('#'); i++; }
			else
				putchar(' ');
		putchar('\n');
	}

	return 0;
}
