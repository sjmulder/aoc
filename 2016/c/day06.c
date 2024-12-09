#include <stdio.h>

int
main(int argc, char **argv)
{
	char buf[16], p1[9]="aaaaaaaa", p2[9]="aaaaaaaa";
	int counts[8][256]={}, i,j;

	if (argc > 1)
		freopen(argv[1], "r", stdin);

	while (fgets(buf, sizeof(buf), stdin))
		for (i=0; i<8 && buf[i] >= 'a' && buf[i] <= 'z'; i++)
			counts[i][(int)buf[i]]++;

	for (i=0; i<8; i++)
	for (j='a'; j<='z'; j++) {
		if (counts[i][j] > counts[i][(int)p1[i]]) p1[i] = j;
		if (counts[i][j] < counts[i][(int)p2[i]]) p2[i] = j;
	}

	printf("06: %s %s\n", p1, p2);
	return 0;
}
