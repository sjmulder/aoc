#include "../compat/builtin.h"

#ifdef COMPAT_POPCOUNT
int
popcount(unsigned u)
{
	int n=0;

	while (u)
		{ n += u&1; u >>= 1; }

	return n;
}
#endif

#ifdef COMPAT_CTZ
int
ctz64(uint64_t u)
{
	int n=0;

	if (!u)
		return 0; /* undefined */
	while (!(u&1))
		{ n++; u >>= 1; }

	return n;
}
#endif
