#include <stdio.h>
#include <string.h>
#include <err.h>

struct fish { int d,n; }; /* depth, count */

#define SZ 1000

static struct fish fs[SZ];
static int nf;

static void
dump(void)
{
	int i;

	for (i=0; i<nf; i++) printf("%x", fs[i].d); putchar('\n');
	for (i=0; i<nf; i++) printf("%x", fs[i].n); putchar('\n');
	putchar('\n');
}

static void
dump_wide(void)
{
	int i;

	for (i=0; i<nf; i++) printf("%5d", fs[i].d); putchar('\n');
	for (i=0; i<nf; i++) printf("%5d", fs[i].n); putchar('\n');
	putchar('\n');
}

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
			if (++nf > SZ) errx(1, "overflow");
			memmove(fs+i+1, fs+i, (nf-i-1)*sizeof(*fs));
			fs[i].d   = ++fs[i+1].d;
			fs[i].n   = fs[i+1].n/2;
			fs[i+1].n = fs[i+1].n-fs[i].n;
			return 1;
		}
	
	return 0;
}

static void
reduce(void)
{
	while (1) {
		if (explode()) { printf(" explode:\n"); dump(); } else
		if (split()) { printf(" split:\n"); dump(); } else
			break;
	}
}

static void
score(void)
{
	int i;

	dump_wide();

	while (nf>1)
		for (i=0; i<nf-1; i++) {
			if (fs[i].d != fs[i+1].d) continue;
			fs[i].n = 3*fs[i].n + 2*fs[i+1].n;
			fs[i].d--;
			nf--;
			memmove(fs+i+1, fs+i+2, (nf-i-1)*sizeof(*fs));
			dump_wide();
		}

}

int
main()
{
	static char buf[100];
	int i,d=0;

	while (fgets(buf, sizeof(buf), stdin)) {
		for (i=0; i<nf; i++)
			fs[i].d++;
		d = nf>1;
		for (i=0; buf[i]; i++)
			if (buf[i] == '[') d++; else
			if (buf[i] == ']') d--; else
			if (buf[i] >= '0' && buf[i] <= '9') {
				fs[nf].n = buf[i]-'0';
				fs[nf].d = d;
				if (++nf >= SZ) errx(1, "overflow");
			}
		dump();
		reduce();
	}

	dump();
	score();
	return 0;
}
