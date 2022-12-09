#include <string.h>

#ifdef _WIN32
# define COMPAT_STRSEP
#endif

#ifdef COMPAT_STRSEP
char *strsep(char **stringp, const char *delim);
#endif
