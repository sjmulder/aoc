#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define MEMCAP (1L<<20)

/* hash table w/ open addressing */
struct cell { long addr, val; };
static struct cell mem[MEMCAP];

static long cur_ones, cur_floats;
static long sum; /* updated live */

static struct cell *
getcell(long addr)
{
	long off=0,i;

	off = addr % MEMCAP;

	/* open addressing */
	for (i=0; i < MEMCAP; i++) {
		if (mem[off].addr == addr)	/* match */
			return &mem[off];
		if (!mem[off].addr) {		/* unclaimed */
			mem[off].addr = addr;
			return &mem[off];
		}
		if (++off == MEMCAP)
			off = 0;
	}

	errx(1, "out of cells");
}

static void
op_mask(char *val)
{
	for (cur_ones = cur_floats = 0; *val; val++) {
		cur_ones   = (cur_ones   << 1) | (*val == '1');
		cur_floats = (cur_floats << 1) | (*val == 'X');
	}
}

static void
op_set(long addr, long floats, long val)
{
	struct cell *cell;
	long bit;

	if (floats) {
		bit = 1L << __builtin_ctzl(floats);
		op_set(addr & ~bit, floats & ~bit, val);
		op_set(addr |  bit, floats & ~bit, val);
	} else {
		cell = getcell(addr);
		cell->addr = addr;
		sum -= cell->val ;
		sum += cell->val = val;
	}
}

int
main()
{
	static char buf[128];
	char *c;
	long addr,val;

	while (fgets(buf, 128, stdin)) {
		if (!(c = strchr(buf, '\n')))
			errx(1, "line too long: %s", buf);
		*c = '\0';

		if (!strncmp(buf, "mask", 4)) {
			if (!(c = strchr(buf, '=')))
				errx(1, "no =: %s", buf);
			if (!c[2])
				errx(1, "bad mask: %s", buf);

			op_mask(c+2);
		} else if (!strncmp(buf, "mem", 3)) {
			if (sscanf(buf, "mem[%ld", &addr) != 1)
				errx(1, "bad addr: %s", buf);
			if (!(c = strchr(buf, '=')))
				errx(1, "no =: %s", buf);
			if (sscanf(c, " = %ld", &val) != 1)
				errx(1, "bad val: %s", buf);

			op_set(addr | cur_ones, cur_floats, val);
		} else
			errx(1, "bad line: %s", buf);
	}

	printf("%ld\n", sum);
}
