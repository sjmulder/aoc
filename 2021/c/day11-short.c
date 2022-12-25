#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SZ	10
#define ON	('9'+2)

static char g[SZ][SZ+1];

static void flash(int r, int c) {
	int r2,c2;

	if (g[r][c] == ON) return;
	g[r][c] = ON;

	for (r2=r-1; r2<=r+1; r2++) for (c2=c-1; c2<=c+1; c2++) {
		if (r2<0 || r2>=SZ) continue;
		if (c2<0 || c2>=SZ) continue;
		if (r==r2 && c==c2) continue;
		if (g[r2][c2] <= '9' && ++g[r2][c2] > '9') flash(r2, c2);
	}
}

int main() {
	int i,r,c, p1=0, nf=0;

	if (fread(g, 1, sizeof(g), stdin) != 1)
		assert(!"bad input");

	for (i=0; nf != SZ*SZ; i++) {
		nf=0;
		for (r=0;r<SZ;r++) for (c=0;c<SZ;c++) g[r][c]++;
		for (r=0;r<SZ;r++) for (c=0;c<SZ;c++) if (g[r][c]>'9') flash(r, c);
		for (r=0;r<SZ;r++) for (c=0;c<SZ;c++) if (g[r][c]==ON) {nf++; g[r][c]='0';}
		if (i<100) p1 += nf;
	}

	printf("11: %d %d\n", p1, i);
	return 0;
}
