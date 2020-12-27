#if defined(_MSC_VER) && _MSC_VER <= 1200

typedef __int64 int64_t;

typedef unsigned __int64 uint64_t;

#else
# include <stdint.h>
#endif
