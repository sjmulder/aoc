#include "common.h"

#define GZ	146

int
main(int argc, char **argv)
{
	static char g[GZ][GZ];
	static const char w[] = "XMAS";
	int p1=0,p2=0, x,y, m,i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (y=3; y<GZ && fgets(g[y]+3, GZ-3, stdin); y++) ;

	for (y=3; y<GZ-3; y++)
	for (x=3; x<GZ-3; x++) {
		for (m=1,i=0; i<4; i++) {m &= g[y+i][x]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y-i][x]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y][x+i]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y][x-i]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y+i][x+i]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y-i][x-i]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y+i][x-i]==w[i];} p1+=m;
		for (m=1,i=0; i<4; i++) {m &= g[y-i][x+i]==w[i];} p1+=m;

		p2 += g[y+1][x+1]=='A' &&
		      ((g[y][x]  =='M' && g[y+2][x+2]=='S')  ||
		       (g[y][x]  =='S' && g[y+2][x+2]=='M')) &&
		      ((g[y+2][x]=='M' && g[y][x+2]  =='S')  ||
		       (g[y+2][x]=='S' && g[y][x+2]  =='M'));
	}

	printf("04: %d %d\n", p1, p2);
}
