#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "compat.h"

#undef min
#undef max

static uint64_t p1, p2;
static uint64_t bs_pos, bs_buf;
static int bs_buflen;

static uint64_t min(uint64_t a, uint64_t b) { return a<b ? a : b; }
static uint64_t max(uint64_t a, uint64_t b) { return a>b ? a : b; }

static uint64_t
read_bits(int nbits)
{
	uint64_t c, ret;

	while (bs_buflen < nbits) {
		if (bs_buflen + 4 > 64) errx(1, "bit buffer full");
		if (scanf("%1"PRIx64, &c) != 1) errx(1, "bad char");
		bs_buflen += 4;
		bs_buf = bs_buf << 4 | c;
	}

	ret = bs_buf >> (bs_buflen - nbits);
	bs_pos += nbits;
	bs_buflen -= nbits;
	bs_buf &= ((uint64_t)1 << bs_buflen) -1;

	return ret;
}

static uint64_t
eval_next(void)
{
	uint64_t type, val=0,chunk, start,bits=0,num=0, i;

	p1 += read_bits(3);
	type = read_bits(3);

	if (type == 4)
		do {
			chunk = read_bits(5);
			val = val << 4 | (chunk & 15);
			val |= chunk & 5;
		} while (chunk >> 4);
	else if (read_bits(1))
		num = read_bits(11);
	else
		bits = read_bits(15);

	if (!num && !bits)
		return val;

	start = bs_pos;
	val = eval_next();

	for (i=1; (num && i<num) || (bits && bs_pos < start+bits); i++)
		switch (type) {
		case 0: val += eval_next(); break;
		case 1: val *= eval_next(); break;
		case 2: val = min(val, eval_next()); break;
		case 3: val = max(val, eval_next()); break;
		case 5: val = val > eval_next(); break;
		case 6: val = val < eval_next(); break;
		case 7: val = val == eval_next(); break;
		}

	return val;
}

int
main()
{
	p2 = eval_next();
	printf("16: %"PRIu64" %"PRIu64"\n", p1, p2);
	return 0;
}
