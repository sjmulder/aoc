#include <stdint.h>
#include "softint.h"

const struct u64 u64_pow10[20] = {
    {0x00000001, 0x00000000},
    {0x0000000a, 0x00000000},
    {0x00000064, 0x00000000},
    {0x000003e8, 0x00000000},
    {0x00002710, 0x00000000},
    {0x000186a0, 0x00000000},
    {0x000f4240, 0x00000000},
    {0x00989680, 0x00000000},
    {0x05f5e100, 0x00000000},
    {0x3b9aca00, 0x00000000},
    {0x540be400, 0x00000002},
    {0x4876e800, 0x00000017},
    {0xd4a51000, 0x000000e8},
    {0x4e72a000, 0x00000918},
    {0x107a4000, 0x00005af3},
    {0xa4c68000, 0x00038d7e},
    {0x6fc10000, 0x002386f2},
    {0x5d8a0000, 0x01634578},
    {0xa7640000, 0x0de0b6b3},
    {0x89e80000, 0x8ac72304}
};

struct u64
u64_mk(uint32_t hi, uint32_t lo)
{
	struct u64 ret;

	ret.lo = lo;
	ret.hi = hi;

	return ret;
}

struct u64
u64_add(struct u64 a, struct u64 b)
{
	struct u64 ret;

	ret.lo = a.lo + b.lo;
	ret.hi = a.hi + b.hi + (ret.lo < a.lo || ret.lo < b.lo);

	return ret;
}

struct u64
u64_sub(struct u64 a, struct u64 b)
{
	struct u64 ret;

	ret.lo = a.lo - b.lo;
	ret.hi = a.hi - b.hi - (ret.lo > a.lo);

	return ret;
}

struct u64
u64_add32(struct u64 a, uint32_t b)
{
	struct u64 ret;

	ret.lo = a.lo + b;
	ret.hi = a.hi + (ret.lo < a.lo);

	return ret;
}

struct
u64 u64_sub32(struct u64 a, uint32_t b)
{
	struct u64 ret;

	ret.lo = a.lo - b;
	ret.hi = a.hi - (ret.lo > a.lo);

	return ret;
}

int
u64_eq(struct u64 a, struct u64 b)
{
	return a.hi == b.hi && a.lo == b.lo;
}

int
u64_lte(struct u64 a, struct u64 b)
{
	return a.hi < b.hi || (a.hi == b.hi && a.lo <= b.lo);
}

const char *
u64_fmt(struct u64 a)
{
	static char s[21];
	int i;

	for (i=0; i<20; i++)
	for (s[i] = '0'; u64_lte(a, u64_pow10[20-1-i]); s[i]++)
		a = u64_sub(a, u64_pow10[20-1-i]);

	s[20] = '\0';

	for (i=0; s[i]=='0'; i++)
		;
	
	return &s[i];
}
