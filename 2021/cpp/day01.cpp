#include <iostream>
#include <array>

template<typename T>
constexpr auto mod(T a, T b) {
	return ((a%b) + b) % b;
}

template<typename T, size_t Sz>
class ring {
	std::array<T, Sz> arr;
public:
	T& operator[](size_t i) {
		return arr[mod(i, Sz)];
	}
	const T& operator[](size_t i) const {
		return arr[mod(i, Sz)];
	}
};


/*
 * a[] is a 4-element ring buffer. Multiplication is used instead of
 * '&&' to make this branchless. The '(n+3)%4' actually means '(n-1)%4'
 * but that would yield some negative indices.
 */

int
main()
{
	ring<int, 4> a;
	int n=0, ninc1=0, ninc3=0;

	for (; std::cin >> a[n]; n++) {
		ninc1 += (n>0) * (a[n] > a[n-1]);
		ninc3 += (n>2) * (a[n] > a[n-3]);
	}

	std::cout << "01: " << ninc1 << " " << ninc3 << "\n";
}
