#include <stdio.h>
#include <string.h>

static char buf[1024*1024];
static size_t sz, w;

static size_t f(int dx, int dy) {
	size_t c,r,i,n=0;
	for (c=r=i=0; i<sz; i = (r+=dy)*(w+1) + (c=(c+dx)%w))
		n += buf[i] == '#';
	return n;
}

int main() {
	sz = (size_t)fread(buf, 1, sizeof(buf), stdin);
	w = (size_t)(strchr(buf, '\n') - buf);
	printf("%zu %zu\n", f(3,1), f(1,1)*f(3,1)*f(5,1)*f(7,1)*f(1,2));
}
