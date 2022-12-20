#include <stdio.h>
#include <memory.h>
#include <assert.h>

#define LEN(a)	((int)(sizeof(a)/sizeof(*(a))))

#define IDX_OFF	10240

int sgn(int x) { return x<0 ? -1 : x>0 ? 1 : 0; }
int wrap(int x, int n) { return (x%n +n) %n; }

int
main()
{
	static int a[5120], b[5120], idx[20480];
	int n=0, i,j,d, jw,jdw, val, cur, tmp;

	while (scanf("%d", &a[n]) == 1) {
		assert(a[n]+IDX_OFF >= 0);
		assert(a[n]+IDX_OFF < LEN(idx));
		idx[a[n]+IDX_OFF] = n;
		n++;
		assert(n+1 < LEN(a));
	}

	for (i=0; i<n; i++)
		printf("[%6d] %6d\n", idx[a[i]+IDX_OFF], a[i]);
	for (i=0; i<n; i++)
		assert(i == idx[a[i]+IDX_OFF]);

	printf("---\n");
	printf("memcpy\n");
	memcpy(b, a, sizeof(b));

	for (i=0; i<n; i++)
		printf("[%6d] %6d\n", idx[b[i]+IDX_OFF], b[i]);
	for (j=0; j<n; j++)
		assert(j == idx[b[j]+IDX_OFF]);

	for (i=0; i<n; i++) {
		val = a[i];
		cur = idx[val+IDX_OFF];
		d = sgn(val);

		printf("---\n");
		printf("moving %d from %d to %d\n", val, cur, cur+val);

		for (j=cur; j!=cur+val; j+=d) {
			jw = wrap(j, n);
			jdw = wrap(j+d, n);

			idx[b[jw]+IDX_OFF] = jdw;
			idx[b[jdw]+IDX_OFF] = jw;

			tmp = b[jw];
			b[jw] = b[jdw];
			b[jdw] = tmp;
		}

		for (j=0; j<n; j++)
			printf("[%6d] %6d\n", idx[b[j]+IDX_OFF], b[j]);
		for (j=0; j<n; j++)
			assert(j == idx[b[j]+IDX_OFF]);
	}


	printf("20: %d\n",
	    b[wrap(idx[0+IDX_OFF] + 1000, n)] +
	    b[wrap(idx[0+IDX_OFF] + 2000, n)] +
	    b[wrap(idx[0+IDX_OFF] + 3000, n)]);

	return 0;
}
