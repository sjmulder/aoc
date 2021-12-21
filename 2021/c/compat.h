#if defined(_MSC_VER)
# define noreturn __declspec(noreturn)
#elif defined(__GNUC__)
# define noreturn __attribute__((noreturn))
#else
# include <stdnoreturn.h>
#endif

#ifdef NO_STRSEP
char *strsep(char **stringp, const char *delim);
#endif

#ifdef NO_ERR
noreturn void err(int status, const char *fmt, ...);
noreturn void errx(int status, const char *fmt, ...);
#else
#include <err.h>
#endif
