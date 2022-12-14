#include <string.h>

#if defined(_WIN32) || defined(__TURBOC__)
# define COMPAT_STRSEP
#endif

#ifdef COMPAT_STRSEP
char *strsep(char **stringp, const char *delim);
#endif
