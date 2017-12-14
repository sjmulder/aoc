#define KHASH_SZ 16

struct khash_state {
	char arr[256];
	int pos;
	int skip;
};

void khash_begin(struct khash_state *state);
void khash_append(struct khash_state *state, const char *data, int n);
void khash_build(struct khash_state *state, char *dest);
