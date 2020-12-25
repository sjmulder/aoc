#include "../compat/builtin.h"

#ifdef COMPAT_POPCOUNT
int
popcount(unsigned u)
{
	int n=0;

	while (u) {
		n += u&1;
		u >>= 1;
	}

	return n;
}
#endif
