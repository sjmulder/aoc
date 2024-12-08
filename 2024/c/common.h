#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>

#define DISCARD(x)	((void)!(x))
#define MAX(a,b)	((a)>(b)?(a):(b))
#define MIN(a,b)	((a)<(b)?(a):(b))
#define LEN(a)		(sizeof(a)/sizeof(*(a)))

#if defined(__TURBOC__)
# define UNUSED
# define NO_STDINT
# define NO_STDCKDINT
# define NO_INTTYPES
# define NO_MEMORY
# define NO_SNPRINTF
# define NO_STRSEP
# define NO_BUILTIN_OVERFLOW
#elif defined(_MSC_VER) && _MSC_VER <= 1200
# define UNUSED
# define NO_STDINT
# define NO_STDCKDINT
# define NO_INTTYPES
# define NO_STRSEP
# define NO_STRTOLL
# define NO_BUILTIN_OVERFLOW
# define snprintf _snprintf	/* note: returns 1 on overflow */
# define atoll(s) strtoll((s), NULL, 10);
# undef max
# undef min
#elif defined(_MSC_VER) && _MSC_VER <= 1942
# define NO_STRSEP
# define NO_CKDINT
# define snprintf _snprintf	/* note: returns 1 on overflow */
#else
# define UNUSED __attribute__((unused))
#endif

#ifndef NO_INTTYPES
# include <inttypes.h>
#else
# define PRIi64 "I64i"
# define PRId64 "I64d"
# define PRIu64 "I64u"
# define SCNi64 "I64i"
# define SCNd64 "I64d"
# define SCNu64 "I64u"
#endif

#if !defined(NO_STDINT)
# include <stdint.h>
#elif defined(__TURBOC__)
typedef signed char int8_t;
typedef unsigned char uint8_t;
#else
# define INT64_MAX  0x7FFFFFFFFFFFFFFF
# define UINT64_MAX 0xFFFFFFFFFFFFFFFF
typedef __int8 int8_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int64 uint64_t;
#endif

#if !defined(NO_STDCKDINT)
# include <stdckdint.h>
#elif !defined(NO_BUILTIN_OVERFLOW)
# define ckd_add(r,a,b) __builtin_add_overflow((a), (b), (r))
# define ckd_mul(r,a,b) __builtin_mul_overflow((a), (b), (r))
#else
/* macros are generic, but uin64_t will do for here */
uint64_t ckd_add(uint64_t a, uint64_t b, uint64_t *r);
uint64_t ckd_mul(uint64_t a, uint64_t b, uint64_t *r);
#endif

#ifndef NO_MEMORY
# include <memory.h>
#endif

#ifdef NO_SNPRINTF
int snprintf(char *s, size_t sz, const char *fmt, ...);
#endif

#ifdef NO_STRSEP
char *strsep(char **stringp, const char *delim);
#endif

#ifdef NO_STRTOLL
int64_t strtoll(const char *s, const char **endp, int base);
#endif
