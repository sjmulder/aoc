#include "common.h"

int
main(int argc, char **argv)
{
	char line[128], *rest, *tok;
	long nextra[200]={0}, p1=0,p2=0; /* long for 16 bit platforms */
	int nums[10], nnums;
	int id,val,nmatch, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (id=0; (rest = fgets(line, sizeof(line), stdin)); id++) {
		nnums = nmatch = 0;

		while ((tok = strsep(&rest, " ")) && !strchr(tok, ':'))
			;
		while ((tok = strsep(&rest, " ")) && !strchr(tok, '|'))
			if ((val = atoi(tok)))
				nums[nnums++] = val;
		while ((tok = strsep(&rest, " ")))
			if ((val = atoi(tok)))
				for (i=0; i<nnums; i++)
					nmatch += nums[i] == val;

		for (i=0; i<nmatch; i++)
			nextra[id+1+i] += nextra[id]+1;

		p1 += nmatch ? 1 << (nmatch-1) : 0;
		p2 += nextra[id]+1;
	}

	printf("04: %ld %ld\n", p1, p2);
	return 0;
}
