#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

struct logent {
	int year, month, day, hour, min;
	int guard_no;
	int event;
#define GUARD_BEGINS 0
#define GUARD_SLEEPS 1
#define GUARD_WAKES 2
};

struct guard {
	int no;
	int sleep_mins;
	int sleep_map[60];
};

static size_t read_log(struct logent *, size_t);
static int compar_logent(const void *, const void *);
static struct guard *get_guard(int, struct guard *, size_t, size_t *);

int
main()
{
	static struct logent log[16*1024];
	static struct guard guards[256];

	size_t i, nlogs, nguards = 0;
	struct guard *guard = NULL, *sleepiest = NULL;
	int state = -1, sleep_start = -1;
	int j, max_min = 0;

	if (!(nlogs = read_log(log, LEN(log))))
		errx(1, "no logs");
	qsort(log, nlogs, sizeof(log[0]), compar_logent);

	if (log[0].event != GUARD_BEGINS)
		errx(1, "wrong first log event: %d", log[0].event);

	for (i = 0; i < nlogs; i++) {
		fprintf(stderr, "%04d-%02d-%02d %02d:%02d %d %d\n",
		    log[i].year, log[i].month, log[i].day,
		    log[i].hour, log[i].min,
		    log[i].event, log[i].guard_no);

		switch (log[i].event) {
		case GUARD_BEGINS:
			guard = get_guard(log[i].guard_no, guards,
			    LEN(guards), &nguards);
			fprintf(stderr, " got guard %p\n", guard);
			break;
		case GUARD_SLEEPS:
			if (state == GUARD_SLEEPS)
				errx(1, "wrong state for SLEEPS");
			sleep_start = log[i].min;
			break;
		case GUARD_WAKES:
			if (state != GUARD_SLEEPS)
				errx(1, "wrong state for WAKES");
			guard->sleep_mins += log[i].min - sleep_start;
			for (j = sleep_start; j < log[i].min; j++)
				guard->sleep_map[j]++;
			fprintf(stderr, " sleep_mins=%d\n",
			    guard->sleep_mins);
			if (!sleepiest || guard->sleep_mins >
			    sleepiest->sleep_mins) {
				fprintf(stderr, " new sleepiest\n");
				sleepiest = guard;
			}
			break;
		default:
			errx(1, "unexpected event %d", log[i].event);
		}

		state = log[i].event;
	}

	if (state != GUARD_WAKES)
		errx(1, "unexpected end state: %d", state);
	if (!sleepiest)
		errx(1, "no solution");

	for (j = 1; j < 60; j++)
		if (sleepiest->sleep_map[j] >
		    sleepiest->sleep_map[max_min])
			max_min = j;

	fprintf(stderr, "sleepiest minute=%d (%d times)\n",
	    max_min, sleepiest->sleep_map[max_min]);
	
	printf("%d\n", max_min * sleepiest->no);
	return 0;
}

static size_t
read_log(struct logent *log, size_t sz)
{
	size_t i = 0;
	char msg[32];
	int n;

	while (1) {
		if (i >= sz)
			err(1, "too many logents");
		n = scanf("[%d-%d-%d %d:%d] ",
		    &log[i].year, &log[i].month, &log[i].day,
		    &log[i].hour, &log[i].min);
		if (n != 5)
			return i;

		if (!fgets(msg, LEN(msg), stdin))
			err(1, "fgets");

		n = sscanf(msg, "Guard #%d", &log[i].guard_no);
		if (n == 1)
			log[i].event = GUARD_BEGINS;
		else if (strstr(msg, "wakes up"))
			log[i].event = GUARD_WAKES;
		else if (strstr(msg, "falls asleep"))
			log[i].event = GUARD_SLEEPS;
		else
			errx(1, "can't parse msg: %s", msg);

		i++;
	}
}

static int
compar_logent(const void *a, const void *b)
{
	const struct logent *la, *lb;

	la = a;
	lb = b;

	if (la->year - lb->year)
		return la->year - lb->year;
	if (la->month - lb->month)
		return la->month - lb->month;
	if (la->day - lb->day)
		return la->day - lb->day;
	if (la->hour - lb->hour)
		return la->hour - lb->hour;
	if (la->min - lb->min)
		return la->min - lb->min;

	return 0;
}

static struct guard *
get_guard(int no, struct guard *guards, size_t sz, size_t *len)
{
	size_t i;

	for (i = 0; i < *len; i++)
		if (guards[i].no == no)
			return &guards[i];

	if (*len >= sz)
		errx(1, "out of guard slots");

	guards[*len].no = no;

	return &guards[(*len)++];
}
