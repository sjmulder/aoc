#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>

static int run(char *, char *, size_t);
static void increment(char *, size_t);
static int validate(char *, size_t);

int
main(int argc, char **argv)
{
	char *from, *to;
	char job_from[64], job_to[64];
	char first;
	size_t len;
	int nvalid = 0, job_nvalid;
	int comms[2], status;

	if (argc != 3)
		errx(1, "usage: %s <from> <to>", argv[0]);

	from = argv[1];
	to = argv[2];
	len = strlen(from);

	assert(len);
	assert(len == strlen(to));
	assert(len <= 64);

	if (pipe(comms) == -1)
		err(1, "pipe");

	for (first = from[0]; first <= to[0]; first++) {
		if (first == from[0]) 
			memcpy(job_from, from, len);
		else
			memset(job_from, first, len);

		if (first == to[0])
			memcpy(job_to, to, len);
		else {
			job_to[0] = first+1; /* yes, to '9'+1 */;
			memset(job_to+1, '0', len-1);
		}

		switch (fork()) {
			case -1: err(1, "fork");
			case 0: break;
			default: continue;
		}

		job_nvalid = run(job_from, job_to, len);

		fprintf(stderr, " %.*s-%.*s %d\n", (int)len, job_from,
		    (int)len, job_to, job_nvalid);

		if (write(comms[1], &job_nvalid, sizeof(int)) == -1)
			err(1, "write");

		return 0;
	}

	if (close(comms[1]) == -1)
		err(1, "close");

	while (1) {
		switch (read(comms[0], &job_nvalid, sizeof(int))) {
			case -1: err(1, "read");
			case 0: goto done_reading;
		}
		nvalid += job_nvalid;
	}
done_reading:

	while (wait(&status) != -1)
		if (status)
			errx(1, "child failed");
	if (errno != ECHILD)
		err(1, "wait");

	printf("%d\n", nvalid);
}

static int
run(char *from, char *to, size_t len)
{
	char num[64];
	int nvalid = 0;

	memcpy(num, from, len);

	while (memcmp(num, to, len) < 0) {
		if (validate(num, len))
			nvalid++;
		increment(num, len);
	}

	return nvalid;
}

static void
increment(char *num, size_t len)
{
	size_t digit = len-1;

	while (num[digit] == '9') {
		assert(digit);
		/*
		 * Normally one'd reset to 0, but we can skip all up
		 * to num[digit-1] since decrementing digits are
		 * invalid solutions.
		 */
		num[digit] = num[digit-1];
		digit--;
	}

	num[digit]++;
}

static int
validate(char *num, size_t len)
{
	size_t i;
	int group_len = 1, have_double = 0;

	for (i = 0; i < len-1; i++) {
		if (num[i] > num[i+1])
			return 0;

		if (num[i] == num[i+1])
			group_len++;
		else {
			if (group_len == 2)
				have_double = 1;
			group_len = 1;
		}
	}

	if (group_len == 2)
		have_double = 1;

	return have_double;
}
