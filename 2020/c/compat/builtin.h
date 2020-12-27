#include "../compat/stdint.h"

#if defined(_MSC_VER) && _MSC_VER <= 1200
# define COMPAT_POPCOUNT
#elif defined(_MSC_VER)
# define popcount(x) __popcnt(x)
#else
# define popcount(x) __builtin_popcount(x)
#endif

#if defined(_MSC_VER)
# define COMPAT_CTZ
#else
# define ctz64(x) __builtin_ctzll(x)
#endif

#ifdef COMPAT_POPCOUNT
int popcount(unsigned);
#endif

#ifdef COMPAT_CTZ
int ctz64(uint64_t);
#endif
