#include <iostream>
#include <vector>

template<typename T>
static constexpr T
from_binary(std::string s)
{
	T acc = 0;

	for (auto c : s)
		acc = (acc << 1) + (c == '1');

	return acc;
}

static size_t
sort_bybitval(std::vector<uint16_t> ns, int len, int bit, bool set)
{
	size_t c = 0;

	for (int i=0; i<len; i++)
		if (((ns[i]>>bit) & 1) == set)
			std::swap(ns[c++], ns[i]);

	return c;
}

static uint16_t
get_measurement(std::vector<uint16_t> ns, int nbits, bool use_epsilon)
{
	auto len = ns.size();

	for (auto bit = nbits-1; bit>=0 && len>1; bit--) {
		size_t count=0;
		for (size_t i=0; i<len; i++)
			count += (ns[i]>>bit) & 1;
		auto val = (count >= (len+1)/2) ^ use_epsilon;
		len = sort_bybitval(ns, len, bit, val);
	}

	return ns[0];
}

int
main()
{
	std::vector<uint16_t> nums;
	size_t nbits=0;
	uint16_t gamma=0, epsilon, oxy, co2;

	std::string binary;
	while (std::cin >> binary) {
		if (!nbits)
			nbits = binary.size();
		nums.push_back(from_binary<uint16_t>(binary));
	}

	for (size_t bit=0; bit<nbits; bit++) {
		size_t ones = 0;
		for (size_t i=0; i < nums.size(); i++)
			if (nums[i] & (1<<bit) &&
			    ++ones >= (nums.size()+1)/2)
				{ gamma |= 1<<bit; break; }
	}

	epsilon = ~gamma & ((1<<nbits)-1);
	oxy = get_measurement(nums, nbits, false);
	co2 = get_measurement(nums, nbits, true);

	std::cout << "03: " << gamma*epsilon << " " << oxy*co2 << "\n";
}
