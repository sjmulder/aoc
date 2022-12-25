#if defined(__TURBOC__) || defined(_MSC_VER) && _MSC_VER <= 1200
# define UNUSED
#else
# define UNUSED __attribute__((unused))
#endif

#ifdef max
#undef max
#endif
