#include <stdio.h>
#include <string.h>
#include <err.h>

#define LEN(a) ((int)((sizeof(a)/sizeof(*(a)))))

struct fish { int d,n; }; /* depth, value */

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
	int i, any=1;

	while (nf>1 && any)
		for (i=0, any=0; i<nf-1; i++) {
			if (fs[i].d != fs[i+1].d) continue;
			fs[i].n = 3*fs[i].n + 2*fs[i+1].n;
			fs[i].d--;
			nf--; any=1;
			memmove(fs+i+1, fs+i+2, (nf-i-1)*sizeof(*fs));
		}

	return nf==1 ? fs[0].n : 0;
}

static void
add_str(const char *s)
{
	int i,d;

	for (i=0; i<nf; i++)
		fs[i].d++;
	for (i=0, d=nf>1; s[i]; i++)
		if (s[i] == '[') d++; else
		if (s[i] == ']') d--; else
		if (s[i] >= '0' && s[i] <= '9') {
			fs[nf].n = s[i]-'0';
			fs[nf].d = d;
			if (++nf >= LEN(fs)) errx(1, "overflow");
		}
	while (explode() || split()) ;
}

int
main()
{
	static char ls[101][100];
	int nl=0, i,j,sc, p1=0,p2=0;

	while (fgets(ls[nl], sizeof(ls[nl]), stdin)) {
		add_str(ls[nl]);
		if (++nl >= LEN(ls)) errx(1, "lines overflow");
	}

	p1 = score();

	for (i=0; i<nl; i++)
	for (j=0; j<nl; j++) {
		if (i==j) continue;
		nf = 0;
		add_str(ls[i]);
		add_str(ls[j]);
		if ((sc=score())>p2) p2 = sc;
	}

	printf("18: %d %d\n", p1, p2);
	return 0;
}
