#include <stdio.h>
#include <string.h>

int
main()
{
	static char b[1024*1024];
	int p1=0, bad=0, vow=0, dbl=0;
	size_t sz, i;

	sz = fread(b, 1, sizeof(b), stdin);

	for (i=0; i<sz; i++) {
		if (b[i] == '\n') {
			p1 += !bad && vow>2 && dbl;
			bad = vow = dbl = 0;
		} else if (!bad) {
			vow += !!strchr("aeiou", b[i]);
			dbl = dbl || (i && b[i-1] == b[i]);
			bad = i && b[i]-b[i-1] == 1 &&
			    !!strchr("acpx", b[i-1]);
		}
	}

	printf("%d\n", p1);
	return 0;
}
