#include <string.h>

#if defined(_MSC_VER)
# define COMPAT_STRSEP
#endif

#ifdef COMPAT_STRSEP
char *strsep(char **stringp, const char *delim);
#endif
