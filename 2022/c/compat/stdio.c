#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#ifdef __TURBOC__
# include "compat/stdio.h"
#else
# include "../compat/stdio.h"
#endif

#ifdef COMPAT_SNPRINTF
int
snprintf(char *s, size_t sz, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsprintf(s, fmt, ap);
	assert(ret < sz);
	va_end(ap);

	return ret;
}
#endif
