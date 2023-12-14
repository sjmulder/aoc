struct u64 { uint32_t lo, hi; };

extern const struct u64 u64_pow10[20];

struct u64 u64_mk(uint32_t hi, uint32_t lo);

struct u64 u64_add(struct u64 a, struct u64 b);
struct u64 u64_sub(struct u64 a, struct u64 b);

struct u64 u64_add32(struct u64 a, uint32_t b);
struct u64 u64_sub32(struct u64 a, uint32_t b);

int u64_eq (struct u64 a, struct u64 b);
int u64_lte(struct u64 a, struct u64 b);

const char *u64_fmt(struct u64 a);
