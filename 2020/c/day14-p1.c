#include <assert.h>
#include "compat/stdint.h"

#define MEMCAP 4096

struct cell { uint64_t addr, val; };

static struct cell mem[MEMCAP];
static int ncells;
static uint64_t mask_and, mask_or;

void
p1_mask(char *val)
{
	for (mask_and = mask_or = 0; *val; val++) {
		mask_and = (mask_and << 1) | (*val == 'X');
		mask_or  = (mask_or  << 1) | (*val == '1');
	}
}

void
p1_set(uint64_t addr, uint64_t val)
{
	int i;

	for (i=0; i < ncells; i++)
		if (mem[i].addr == addr)
			break;

	assert(i != MEMCAP);

	if (i == ncells)
		mem[ncells++].addr = addr;

	mem[i].val = (val & mask_and) | mask_or;
}

uint64_t
p1_memsum(void)
{
	uint64_t sum=0;
	int i;

	for (i=0; i < ncells; i++)
		sum += mem[i].val; 

	return sum;
}

