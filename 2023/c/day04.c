#include "common.h"

#define MAXNUMS		10
#define MAXCARDS	200

int
main(int argc, char **argv)
{
	char line[128], *rest, *tok;
	int nums[MAXNUMS], nnums;
	int ncopy[MAXCARDS] = {0};
	int p1=0,p2=0, id, val, nmatch, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (id=0; (rest = fgets(line, sizeof(line), stdin)); id++) {
		assert(id < MAXCARDS);
		nnums = nmatch = 0;

		while ((tok = strsep(&rest, " ")) && !strchr(tok, ':'))
			;

		while ((tok = strsep(&rest, " ")) && !strchr(tok, '|'))
			if ((val = atoi(tok))) {
				assert(nnums < MAXNUMS);
				nums[nnums++] = val;
			}

		while ((tok = strsep(&rest, " ")))
			if ((val = atoi(tok)))
				for (i=0; i<nnums; i++)
					nmatch += nums[i] == val;

		assert(id + nmatch <= MAXCARDS);

		for (i=0; i<nmatch; i++)
			ncopy[id+1+i] += ncopy[id]+1;

		p1 += nmatch ? 1 << (nmatch-1) : 0;
		p2 += ncopy[id]+1;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
