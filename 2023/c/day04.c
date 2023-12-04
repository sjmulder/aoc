#include "common.h"

#define MAXNUMS		10
#define MAXWINS		25
#define MAXCARDS	200

int
main(int argc, char **argv)
{
	char line[128], *rest, *tok;
	int nums[MAXNUMS], nnums;
	int wins[MAXWINS], nwins;
	int ncopy[MAXCARDS] = {0};
	int p1=0,p2=0, id, nmatch, i,j;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (id=0; (rest = fgets(line, sizeof(line), stdin)); id++) {
		assert(id < MAXCARDS);
		nnums = nwins = nmatch = 0;

		while ((tok = strsep(&rest, " ")))
			if (strchr(tok, ':'))
				break;

		while ((tok = strsep(&rest, " "))) {
			if (strchr(tok, '|'))
				break;
			if (*tok) {
				assert(nnums < MAXNUMS);
				nums[nnums++] = atoi(tok);
			}
		}

		while ((tok = strsep(&rest, " ")))
			if (*tok) {
				assert(nwins < MAXWINS);
				wins[nwins++] = atoi(tok);
			}

		for (i=0; i<nnums; i++)
		for (j=0; j<nwins; j++)
			if (nums[i] == wins[j]) {
				nmatch++;
				break;
			}

		assert(id + nmatch <= MAXCARDS);

		for (i=0; i<nmatch; i++)
			ncopy[id+1+i] += ncopy[id]+1;

		p1 += nmatch ? 1 << (nmatch-1) : 0;
		p2 += ncopy[id]+1;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
