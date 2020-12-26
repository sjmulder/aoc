#include "../compat/stdint.h"

#if defined(_MSC_VER) && _MSC_VER <= 1200
# define COMPAT_POPCOUNT
# define COMPAT_CTZ
#endif

#ifdef COMPAT_POPCOUNT
int popcount(unsigned);
#else
# define popcount(x) __builtin_popcount(x)
#endif

#ifdef COMPAT_CTZ
int ctz64(uint64_t);
#else
# define ctz64(x) __builtin_ctzl(x)
#endif
