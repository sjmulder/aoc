#include <stddef.h>

#ifdef __TURBOC__
# include "compat/string.h"
#else
# include "../compat/string.h"
#endif

#ifdef COMPAT_STRSEP
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