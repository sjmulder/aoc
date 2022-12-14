#ifdef __TURBOC__
# define UNUSED
#else
# define UNUSED __attribute__((unused))
#endif

#ifdef max
#undef max
#endif
