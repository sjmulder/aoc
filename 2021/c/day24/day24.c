#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <err.h>

int64_t monad(int input[14]);

static void
solve(int job)
{
	int input[14], i;

	input[0] = job+1;

	for (input[1] = 9; input[1] > 0; input[1]--)
	for (input[2] = 9; input[2] > 0; input[2]--)
	for (input[3] = 9; input[3] > 0; input[3]--)
	for (input[4] = 9; input[4] > 0; input[4]--) {
		for (i=0; i<5; i++)
			putchar('0' + input[i]);
		printf("000000000\n");
		fflush(stdout);

		for (input[5] = 9; input[5] > 0; input[5]--)
		for (input[6] = 9; input[6] > 0; input[6]--)
		for (input[7] = 9; input[7] > 0; input[7]--)
		for (input[8] = 9; input[8] > 0; input[8]--)
		for (input[9] = 9; input[9] > 0; input[9]--)
		for (input[10] = 9; input[10] > 0; input[10]--)
		for (input[11] = 9; input[11] > 0; input[11]--)
		for (input[12] = 9; input[12] > 0; input[12]--)
		for (input[13] = 9; input[13] > 0; input[13]--)
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
	pid_t pids[9], pid;
	int i, status, ret=0;

	for (i=0; i<9; i++)
		switch ((pids[i] = fork())) {
		case -1: err(1, "fork");
		case 0:  solve(i); return 0;
		}
	
	while (1) {
		if ((pid = wait(&status)) == -1 && errno != ECHILD)
			err(1, "wait");
		if (pid == -1) break;

		for (i=0; i<9; i++)
			if (pids[i] == pid)
				{ pids[i] = 0; break; }

		printf("%d exited %d\n", pid, WEXITSTATUS(status));

		if (WEXITSTATUS(status)) {
			for (i=0; i<9; i++)
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
