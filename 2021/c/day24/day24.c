#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <err.h>

#define NJOBS 8		/* 1 to 8 */

int64_t monad(int input[14]);

static void
solve(int job)
{
	int input[14], i;

	for (input[0] = 1+job; input[0] < 10; input[0] += NJOBS)
	for (input[1] = 1; input[1] < 10; input[1]++)
	for (input[2] = 1; input[2] < 10; input[2]++)
	for (input[3] = 1; input[3] < 10; input[3]++)
	for (input[4] = 1; input[4] < 10; input[4]++)
	for (input[5] = 1; input[5] < 10; input[5]++)
	for (input[6] = 1; input[6] < 10; input[6]++) {
		for (i=0; i<7; i++)
			putchar('0' + input[i]);
		printf("1111111\n");
		fflush(stdout);

		for (input[7] = 1; input[7] < 10; input[7]++)
		for (input[8] = 1; input[8] < 10; input[8]++)
		for (input[9] = 1; input[9] < 10; input[9]++)
		for (input[10] = 1; input[10] < 10; input[10]++)
		for (input[11] = 1; input[11] < 10; input[11]++)
		for (input[12] = 1; input[12] < 10; input[12]++)
		for (input[13] = 1; input[13] < 10; input[13]++)
			if (monad(input) == 0) {
				printf("FOUND ");
				for (i=0; i<14; i++)
					putchar('0' + input[i]);
				putchar('\n');
				fflush(stdout);
			}
	}
}

int
main()
{
	pid_t pids[NJOBS], pid;
	int i, status, ret=0;

	for (i=0; i<NJOBS; i++)
		switch ((pids[i] = fork())) {
		case -1: err(1, "fork");
		case 0:  solve(i); return 0;
		}
	
	while (1) {
		if ((pid = wait(&status)) == -1 && errno != ECHILD)
			err(1, "wait");
		if (pid == -1) break;

		for (i=0; i<NJOBS; i++)
			if (pids[i] == pid)
				{ pids[i] = 0; break; }

		printf("%d exited %d\n", pid, WEXITSTATUS(status));

		if (WEXITSTATUS(status)) {
			for (i=0; i<NJOBS; i++)
				if (pids[i] && kill(pids[i], SIGINT))
					err(1, "kill");
			break;
		}
	}

	while (wait(&status) != -1 || errno != ECHILD)
		if (status)
			ret = 1;

	return ret;
}
