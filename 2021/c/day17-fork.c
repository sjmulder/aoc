#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>
#include <err.h>

#define MAXJOBS 256

static int njobs=1,job;
static int64_t  l,r,b,t;

static int64_t
solve_p2(void)
{
	int64_t nhits=0, vx0,vy0, vx,vy, x,y;

	for (vx0 = (int64_t)sqrt(l) +job; vx0 <= r; vx0 += njobs) {
		for (x=0, vx=vx0; x<l && vx; x+=vx, vx--)
			;
		if (x<l || x>r)
			continue;
		for (vy0=b; vy0<-b; vy0++) {
			if (!(vy0 % 100000))
				printf("[%d] %"PRId64" %"PRId64"\n",
				    job, vx0, vy0);
			x=y=0; vx=vx0; vy=vy0;
			while ((x<l || y>t) && x<=r && y>=b) {
				x += vx; vx -= vx>0;
				y += vy; vy -= 1;
			}
			nhits += x>=l && x<=r && y>=b && y<=t;
		}
	}

	return nhits;
}

int
main(int argc, char *argv[])
{
	static int job_fds[MAXJOBS];
	int c,n, fds[2];
	int64_t nhits, p1,p2=0;

	while ((c = getopt(argc, argv, "j:")) != -1)
		switch (c) {
		case 'j': njobs = atoi(optarg); break;
		default: return -1;
		}

	if (njobs < 1 || njobs > MAXJOBS)
		errx(1, "bad -j");

	n = scanf(
	    "target area: x=%"PRId64"..%"PRId64", "
	    "y=%"PRId64"..%"PRId64"", &l,&r,&b,&t);
	if (n != 4)
		errx(1, "bad input");

	for (job=0; job<njobs; job++) {
		if (pipe(fds) == -1)
			err(1, "pipe()");
		switch (fork()) {
		case -1:
			err(1, "fork()");
		case 0:
			close(fds[0]);
			nhits = solve_p2();
			if (write(fds[1], &nhits, sizeof(nhits)) == -1)
				err(1, "write()");
			return 0;
		default:
			close(fds[1]);
			job_fds[job] = fds[0];
			break;
		}
	}

	p1 = b * (b+1) /2;

	for (job=0; job<njobs; job++) {
		if (read(job_fds[job], &nhits, sizeof(nhits)) == -1)
			err(1, "read()");
		p2 += nhits;
	}

	for (job=0; job<njobs; job++)
		wait(NULL);

	printf("17: %"PRId64" %"PRId64"\n", p1, p2);
	return 0;
}
