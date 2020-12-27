#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "compat/stdint.h"
#include "compat/inttypes.h"

static char buf[1024*1024];
static int sz, w;

static int64_t
f(int dx, int dy)
{
	int i;
	int64_t n=0;

	for (i=0; i*dy*(w+1) < sz-w; i++)
		n += buf[i*dy*(w+1)+(i*dx%w)] == '#';
	
	return n;
}

int
main(int argc, char **argv)
{
	FILE *in;

	in = argc<2 ? stdin : fopen(argv[1], "r");
	assert(in);
	sz = (int)fread(buf, 1, sizeof(buf), in);
	w = (int)(strchr(buf, '\n') - buf);

	printf("%" PRId64 " %" PRId64 "\n",
	     f(3,1),
	     f(1,1)*f(3,1)*f(5,1)*f(7,1)*f(1,2));
	//getchar();
	return 0;
}
