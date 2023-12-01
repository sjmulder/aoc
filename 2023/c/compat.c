#include "common.h"

#ifdef NO_SNPRINTF
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

#ifdef NO_STRSEP
char *
strsep(char **stringp, const char *delim)
{
	int i,j;
	char *s;
	
	if (!*stringp)
		return NULL;

	s = *stringp;

	for (i=0; s[i]; i++) 
	for (j=0; delim[j]; j++)
		if (s[i] == delim[j]) {
			s[i] = '\0';
			*stringp = s+i+1;
			return s;
		}

	*stringp = NULL;
	return s;
}
#endif
