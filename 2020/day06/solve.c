#include <stdio.h>
#include <string.h>

int main() {
	int i, c=0, prev, p1=0, p2=0, groupsz=0;
	char map[256]={};

	while ((prev = c) != EOF)
		if ((c = getchar()) != '\n' && c != EOF)
			map[c]++;
		else if (prev == '\n') {
			for (i='a'; i<='z'; i++) {
				p1 += map[i] > 0;
				p2 += map[i] == groupsz;
			}
			groupsz = 0;
			memset(map, 0, 256);
		} else
			groupsz++;

	printf("%d %d\n", p1, p2);
}
