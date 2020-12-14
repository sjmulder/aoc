#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define MEMCAP 4096

struct cell { long addr, val; };

static struct cell mem[MEMCAP];
static int ncells;
static long mask_and, mask_or;

static void
op_mask(char *val)
{
	for (mask_and = mask_or = 0; *val; val++) {
		mask_and = (mask_and << 1) | (*val == 'X');
		mask_or  = (mask_or  << 1) | (*val == '1');
	}
}

static void
op_set(long addr, long val)
{
	int i;

	for (i=0; i < ncells; i++)
		if (mem[i].addr == addr)
			break;
	if (i == MEMCAP)
		errx(1, "mem overflow");
	if (i == ncells)
		mem[ncells++].addr = addr;

	mem[i].val = (val & mask_and) | mask_or;
}

static long
memsum(void)
{
	long sum=0;
	int i;

	for (i=0; i < ncells; i++)
		sum += mem[i].val; 

	return sum;
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

			op_set(addr, val);
		} else
			errx(1, "bad line: %s", buf);
	}

	printf("%ld\n", memsum());
}
