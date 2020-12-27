#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main(int argc, char **argv)
{
	FILE *f;
	char c;
	int n,tmp, x1=0,y1=0,dir1=0, x2=0,y2=0, wx=10,wy=-1, rot=0;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while(fscanf(f, " %c%d", &c, &n) == 2) {
		/* part 2 */
		switch (c) {
			case 'N': wy -= n; break;
			case 'S': wy += n; break;
			case 'W': wx -= n; break;
			case 'E': wx += n; break;
			case 'R': rot = n; break;
			case 'L': rot = 360-n; break;
			case 'F': x2 += wx*n; y2 += wy*n; break;
		}
		switch (rot) {
			case  90: tmp=wx; wx=-wy; wy= tmp; break;
			case 270: tmp=wx; wx= wy; wy=-tmp; break;
			case 180: wx=-wx; wy=-wy; break;
		}
		rot=0;

		/* part 1 */
		if (c == 'F')
			c = "ESWN"[dir1];
		switch (c) {
			case 'N': y1 -= n; break;
			case 'S': y1 += n; break;
			case 'W': x1 -= n; break;
			case 'E': x1 += n; break;
			case 'R': dir1 = (dir1+  n/90) % 4; break;
			case 'L': dir1 = (dir1+4-n/90) % 4; break;
		}

	}

	printf("%d %d\n", abs(x1)+abs(y1), abs(x2)+abs(y2));
	//getchar();
	return 0;
}