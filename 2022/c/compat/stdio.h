#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER <= 1200
/* note, returns -1 on overflow, unlike real snprintf */
# define snprintf _snprintf
#endif

#ifdef __TURBOC__
# define COMPAT_SNPRINTF
#endif

#ifdef COMPAT_SNPRINTF
int snprintf(char *s, size_t sz, const char *fmt, ...);
#endif
