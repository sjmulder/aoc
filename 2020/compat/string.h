#include <string.h>

#if defined(_MSC_VER) && _MSC_VER <= 1200
# define COMPAT_STRSEP
#endif

#ifdef COMPAT_STRSEP
char *strsep(char **stringp, const char *delim);
#endif