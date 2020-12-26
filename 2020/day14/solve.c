#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

extern uint64_t p2_sum;

void p1_mask(char *val);
void p1_set(uint64_t addr, uint64_t val);
uint64_t p1_memsum(void);

void p2_mask(char *val);
void p2_set(uint64_t addr, uint64_t val);

int
main(int argc, char **argv)
{
	static char buf[128];
	FILE *f;
	char *c;
	int n;
	uint64_t addr,val;

	f = argc<2 ? stdin : fopen(argv[1], "r");
	assert(f);

	while (fgets(buf, 128, f)) {
		c = strchr(buf, '\n');
		assert(c && "line too long");
		*c = '\0';

		if (!strncmp(buf, "mask", 4)) {
			c = strchr(buf, '=');
			assert(c && "no =");
			assert(c[2] && "bad mask");

			p1_mask(c+2);
			p2_mask(c+2);
		} else if (!strncmp(buf, "mem", 3)) {
			n = sscanf(buf, "mem[%" SCNu64, &addr);
			assert(n == 1);
			c = strchr(buf, '=');
			assert(c && "no =");
			n = sscanf(c, " = %" SCNu64, &val);
			assert(n == 1);

			p1_set(addr, val);
			p2_set(addr, val);
		} else
			assert(0 && "bad line");
	}

	printf("%" PRIu64 " %" PRIu64 "\n", p1_memsum(), p2_sum);
	//getchar();
	return 0;
}
