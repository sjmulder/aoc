#include <stdio.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

int
main()
{
	int w,h,d, p1=0, p2=0;

	while (scanf(" %dx%dx%d", &w, &h, &d) == 3) {
		p1 += 2*w*h + 2*w*d + 2*h*d + MIN(w*h, MIN(w*d, h*d));
		p2 += 2*w + 2*h + 2*d - MAX(2*w, MAX(2*h, 2*d)) + w*h*d;
	}

	printf("02: %d %d\n", p1, p2);
	return 0;
}
