#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) ((int)((sizeof(a)/sizeof(*(a)))))

struct fish { int d,n; }; /* depth, count */

static struct fish fs[1000];
static int nf;

static int
explode(void)
{
	int i;

	for (i=1; i<nf; i++)
		if (fs[i-1].d == 5 && fs[i].d == 5) {
			if (i>1)    fs[i-2].n += fs[i-1].n;
			if (i+1<nf) fs[i+1].n += fs[i].n;
			fs[i-1].n = 0;
			fs[i-1].d--;
			nf--;
			memmove(fs+i, fs+i+1, (nf-i)*sizeof(*fs));
			return 1;
		}
	
	return 0;
}

static int
split(void)
{
	int i;

	for (i=0; i<nf; i++)
		if (fs[i].n > 9) {
			if (++nf > LEN(fs)) errx(1, "overflow");
			memmove(fs+i+1, fs+i, (nf-i-1)*sizeof(*fs));
			fs[i].d   = ++fs[i+1].d;
			fs[i].n   = fs[i+1].n/2;
			fs[i+1].n = fs[i+1].n-fs[i].n;
			return 1;
		}
	
	return 0;
}

static int
score(void)
{
	int i;

	while (nf>1)
		for (i=0; i<nf-1; i++) {
			if (fs[i].d != fs[i+1].d) continue;
			fs[i].n = 3*fs[i].n + 2*fs[i+1].n;
			fs[i].d--;
			nf--;
			memmove(fs+i+1, fs+i+2, (nf-i-1)*sizeof(*fs));
		}

	return fs[0].n;
}

int
main()
{
	static char buf[100];
	int i,d=0, p1=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		for (i=0; i<nf; i++)
			fs[i].d++;
		for (i=0, d=nf>1; buf[i]; i++)
			if (buf[i] == '[') d++; else
			if (buf[i] == ']') d--; else
			if (buf[i] >= '0' && buf[i] <= '9') {
				fs[nf].n = buf[i]-'0';
				fs[nf].d = d;
				if (++nf>=LEN(fs)) errx(1, "overflow");
			}
		while (explode() || split()) ;
	}

	p1 = score();

	printf("18: %d\n", p1);
	return 0;
}
