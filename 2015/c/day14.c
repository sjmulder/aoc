#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define CAP 16

const char fmt[] =
"%*s can fly %d km/s for %d seconds, but then must rest for %d "
"seconds.";

int
main(int argc, char **argv)
{
	static struct deer { int kms, fly, rest, dist, pts; } d[CAP];
	int t,i, tend=2503, z=0, maxdist=0, maxpts=0;

	if (argc > 1)
		tend = atoi(argv[1]);

	while (scanf(fmt, &d[z].kms, &d[z].fly, &d[z].rest) == 3)
		if (++z >= CAP)
			errx(1, "out of bounds");

	for (t=0; t<tend; t++) {
		for (i=0; i<z; i++) {
			if (t % (d[i].fly + d[i].rest) < d[i].fly)
				d[i].dist += d[i].kms;
			if (maxdist < d[i].dist)
				maxdist = d[i].dist;
		}
		for (i=0; i<z; i++)
			d[i].pts += d[i].dist == maxdist;
	}

	for (i=0; i<z; i++) {
		if (maxdist < d[i].dist)
			maxdist = d[i].dist;
		if (maxpts < d[i].pts)
			maxpts = d[i].pts;
	}

	printf("14: %d %d\n", maxdist, maxpts);
	return 0;
}
