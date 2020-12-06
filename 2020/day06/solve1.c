#include <stdio.h>
#include <string.h>

int main() {
	int c=0, prev, n=0, p1=0;
	char map[255]={};

	while (prev = c, (c = getchar()) != EOF)
		if (c == '\n' && prev == '\n')
			{ p1 += n; n = 0; memset(map, 0, 255); }
		else if (c >='a' && c<='z' && !map[c])
			{ map[c] = 1; n++; }

	printf("%d\n", p1 + n);
}
