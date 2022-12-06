#include <stdio.h>

static int
step(char *ring, int sz, int pos, char c)
{
	int i,j;

	ring[pos%sz] = c;

	if (pos<sz)
		return 0;

	for (i=0; i<sz-1; i++)
	for (j=i+1; j<sz; j++)
		if (ring[i] == ring[j])
			return 0;

	return pos+1;
}

int
main()
{
	static char ring1[4];
	static char ring2[14];
	int p1=0, p2=0, pos=0, c;

	while ((!p1 || !p2) && (c = getchar()) != EOF) {
		if (!p1) p1 = step(ring1, 4, pos, c);
		if (!p2) p2 = step(ring2, 14, pos, c);
		pos++;
	}

	printf("06: %d %d\n", p1, p2);
	return 0;
}
