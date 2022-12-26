#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>

#if defined(__TURBOC__)
# define UNUSED
# define NO_STDINT
# define NO_INTTYPES
# define NO_SNPRINTF
# define NO_STRSEP
#elif defined(_MSC_VER) && _MSC_VER <= 1200
# define UNUSED
# define NO_STDINT
# define NO_INTTYPES
# define NO_STRSEP
# define snprintf _snprintf	/* note: returns 1 on overflow */
# undef max
# undef min
#else
# define UNUSED __attribute__((unused))
#endif

#ifndef NO_INTTYPES
# include <inttypes.h>
#else
# define PRId64 "I64d"
# define PRIu64 "I64u"
# define SCNd64 "I64d"
# define SCNu64 "I64u"
#endif

#ifndef NO_STDINT
# include <stdint.h>
#else
typedef __int8 int8_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int64 uint64_t;
#endif

#ifdef NO_SNPRINTF
int snprintf(char *s, size_t sz, const char *fmt, ...);
#endif

#ifdef COMPAT_STRSEP
char *strsep(char **stringp, const char *delim);
#endif
