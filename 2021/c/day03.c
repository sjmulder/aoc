#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

static int
sort_bybitval(uint16_t *nums, int len, int bit, int set)
{
	int count=0, i;
	uint16_t tmp;

	for (i=0; i<len; i++)
		if (((nums[i]>>bit) & 1) == set) {
			tmp = nums[count];
			nums[count++] = nums[i];
			nums[i] = tmp;
		}

	return count;
}

static uint16_t
get_gamma(uint16_t *nums, int len, int nbits)
{
	int counts[16]={0}, i,bit;
	uint16_t gamma=0;
	
	for (i=0; i<len; i++)
		for (bit=0; bit<nbits; bit++)
			counts[bit] += (nums[i]>>bit) & 1;

	for (bit=0; bit<nbits; bit++)
		if (counts[bit] >= len-counts[bit])
			gamma |= 1<<bit;

	return gamma;
}

static uint16_t
to_epsilon(uint16_t gamma, int nbits)
{
	return ~gamma & ((1<<nbits)-1);
}

static uint16_t
get_measurement(uint16_t *nums, int len, int nbits, int use_epsilon)
{
	int bit;
	uint16_t bits;

	for (bit=nbits-1; bit>=0 && len>1; bit--) {
		bits = get_gamma(nums, len, nbits);
		if (use_epsilon)
			 bits = to_epsilon(bits, nbits);
		len = sort_bybitval(nums, len, bit, (bits>>bit)&1);
	}

	return nums[0];
}

int
main()
{
	char buf[17];
	uint16_t nums[1024]={0}, gamma, epsilon, oxy, co2;
	int nnums=0, nbits=0, i;

	while (scanf(" %16s", buf) == 1) {
		if (!nbits)
			nbits = strlen(buf);
		for (i=0; i<nbits; i++)
			nums[nnums] |= (buf[i]=='1') << (nbits-1-i);
		if (++nnums >= (int)LEN(nums))
			{ fprintf(stderr, "too many nums"); return 1; }
	}

	gamma = get_gamma(nums, nnums, nbits);
	epsilon = to_epsilon(gamma, nbits);
	oxy = get_measurement(nums, nnums, nbits, 0);
	co2 = get_measurement(nums, nnums, nbits, 1);

	printf("%d %d\n", (int)gamma * epsilon, (int)oxy * co2);
	return 0;
}
