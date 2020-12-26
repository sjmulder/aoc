#include <assert.h>
#include "../compat/stdint.h"
#include "../compat/builtin.h"

#define MEMCAP ((uint64_t)1<<20)

/* hash table w/ open addressing */
struct cell { uint64_t addr, val; };
static struct cell mem[MEMCAP];

static uint64_t cur_ones, cur_floats;

uint64_t p2_sum; /* updated live */

static struct cell *
getcell(uint64_t addr)
{
	uint64_t off=0,i;

	off = addr % MEMCAP;

	/* open addressing */
	for (i=0; ; i++) {
		assert(i < MEMCAP);

		if (mem[off].addr == addr)	/* match */
			return &mem[off];
		if (!mem[off].addr) {		/* unclaimed */
			mem[off].addr = addr;
			return &mem[off];
		}
		if (++off == MEMCAP)
			off = 0;
	}
}

void
p2_mask(char *val)
{
	for (cur_ones = cur_floats = 0; *val; val++) {
		cur_ones   = (cur_ones   << 1) | (*val == '1');
		cur_floats = (cur_floats << 1) | (*val == 'X');
	}
}

static void
p2_set_rec(uint64_t addr, uint64_t floats, uint64_t val)
{
	struct cell *cell;
	uint64_t bit;

	if (floats) {
		bit = (uint64_t)1 << ctz64(floats);
		p2_set_rec(addr & ~bit, floats & ~bit, val);
		p2_set_rec(addr |  bit, floats & ~bit, val);
	} else {
		cell = getcell(addr);
		cell->addr = addr;
		p2_sum -= cell->val ;
		p2_sum += cell->val = val;
	}
}

void
p2_set(uint64_t addr, uint64_t val)
{
	p2_set_rec(addr | cur_ones, cur_floats, val);
}
