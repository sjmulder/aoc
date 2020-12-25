#if defined(_MSC_VER) && _MSC_VER <= 1200
# define COMPAT_POPCOUNT
#else
# define popcount(x) __builtin_popcount(x)
#endif

#ifdef COMPAT_POPCOUNT
int popcount(unsigned);
#endif
