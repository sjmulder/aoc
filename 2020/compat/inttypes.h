#if defined(_MSC_VER) && _MSC_VER <= 1200

#define PRId64 "I64d"
#define PRIu64 "I64u"

#else
# include <inttypes.h>
#endif
