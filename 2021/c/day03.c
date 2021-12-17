#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LEN(a) (sizeof(a)/sizeof(*(a)))

static int
sort_bybitval(uint16_t *ns, int len, int bit, int set)
{
	int c=0, i;
	uint16_t tmp;

	for (i=0; i<len; i++)
		if (((ns[i]>>bit) & 1) == set)
			{ tmp = ns[c]; ns[c++] = ns[i]; ns[i] = tmp; }

	return c;
}

static uint16_t
get_measurement(uint16_t *nums, int len, int nbits, int use_epsilon)
{
	int bit, val, count, i;

	for (bit=nbits-1; bit>=0 && len>1; bit--) {
		for (i=0, count=0; i<len; i++)
			count += (nums[i]>>bit) & 1;
		val = (count >= (len+1)/2) ^ use_epsilon;
		len = sort_bybitval(nums, len, bit, val);
	}

	return nums[0];
}

int
main()
{
	char buf[17];
	uint16_t nums[1024]={0}, gamma=0, epsilon, oxy, co2;
	int nnums=0, nbits=0, ones, i,bit;

	while (scanf(" %16s", buf) == 1) {
		if (!nbits)
			nbits = (int)strlen(buf);
		for (i=0; i<nbits; i++)
			nums[nnums] |= (buf[i]=='1') << (nbits-1-i);
		if (++nnums >= (int)LEN(nums))
			{ fprintf(stderr, "too many nums"); return 1; }
	}

	for (bit=0; bit<nbits; bit++)
		for (ones=0, i=0; i<nnums; i++)
			if (nums[i] & (1<<bit) && ++ones >= (nnums+1)/2)
				{ gamma |= 1<<bit; break; }

	epsilon = ~gamma & ((1<<nbits)-1);
	oxy = get_measurement(nums, nnums, nbits, 0);
	co2 = get_measurement(nums, nnums, nbits, 1);

	printf("03: %d %d\n", (int)gamma * epsilon, (int)oxy * co2);
	return 0;
}
