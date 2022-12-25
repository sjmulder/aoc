#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <err.h>

#define BUFSZ	1024
#define CAP	128
#define MAXJOBS	1024

static int ids[CAP], offs[CAP], nids;
static int njobs=1;
static long t0, step=1;

static void
load(void)
{
	static char buf[BUFSZ];
	long off=0, id;
	char *rest, *field;

	fgets(buf, BUFSZ, stdin);
	rest = fgets(buf, BUFSZ, stdin);

	while ((field = strsep(&rest, ","))) {
		if (nids >= CAP)
			errx(1, "ids overflow");
		if ((id = atol(field))) {
			ids[nids] = id;
			offs[nids] = off;
			nids++;
		}
		if (id > step) {
			step = id;
			t0 = id-off;
		}
		off++;
	}
}

static void
run(int job)
{
	long t;
	int i;

	for (t=t0 + step*job; ; t += step*njobs) {
		for (i=0; i<nids; i++)
			if ((t+offs[i]) % ids[i])
				break;
		if (i == nids)
			break;
	}

	printf("%ld (job %d)\n", t, job);
}

int
main(int argc, char **argv)
{
	int job, c;
	pid_t pids[MAXJOBS];

	while ((c = getopt(argc, argv, "j:")) != -1)
		switch (c) {
			case 'j':
				njobs = atoi(optarg);
				if (njobs < 1 || njobs > MAXJOBS)
					errx(1, "bad -j");
				break;
			default:
				return 1;
		}

	load();

	for (job=0; job < njobs; job++)
		switch ((pids[job] = fork())) {
		case -1:
			err(1, "fork()");
		case 0:
			run(job);
			return 0;
		}

	if (wait(NULL) == -1)
		err(1, "wait()");
	for (job=0; job < njobs; job++)
		if (kill(pids[job], SIGTERM) && errno != ESRCH)
			err(1, "kill()");
	while (wait(NULL) != -1)
		;
	if (errno != ECHILD)
		err(1, "wait()");

	return 0;
}
