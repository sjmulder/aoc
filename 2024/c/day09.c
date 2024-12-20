#include "common.h"

/*
 * First went with a doubly linked list approach, but it was quite verbose
 * and we're dealing with short runs (max 9) anyway.
 */
static char input[20*1000+1];
static short disk[100*1000];
int input_sz, disk_sz;

static void
defrag(int p2)
{
	int a,b, a0=0, run, gap;

	/*
	 * b runs back to front, finding files.
	 * a runs front to back (from first gap, a0), finding gaps.
	 *
	 * For part 1 we short circuit the file length check so it deals
	 * with single tiles.
	 */
	for (b=disk_sz-1; b > 0; b--) {
		/* find and measure next file from back */
		for (; b>0 && !disk[b]; b--) ;
		for (run=1; p2 && b>0 && disk[b-1]==disk[b]; b--, run++) ;

		/* find the first gap */
		for (; a0 < b && disk[a0]; a0++) ;

		/* find a gap large enough */
		for (a=a0, gap=0; a<b; a++)
			if (!disk[a]) {
				for (gap=1; disk[a+gap] == disk[a]; gap++) ;
				if (gap >= run) break;
			}

		/* move if its */
		if (gap >= run)
			for (; run > 0; a++, run--) {
				disk[a] = disk[b+run-1];
				disk[b+run-1] = 0;
			}
	}
}

int
main(int argc, char **argv)
{
	int part, i,j;
	uint64_t ans[2]={0,0};

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	input_sz = (int)fread(input, 1, sizeof(input), stdin);
	assert(!ferror(stdin));
	assert(feof(stdin));

	for (part=0; part<2; part++) {
		disk_sz = 0;

		for (i=0; i < input_sz && isdigit(input[i]); i++)
		for (j=0; j < input[i]-'0'; j++) {
			assert(disk_sz < (int)LEN(disk));
			disk[disk_sz++] = i%2 ? 0 : i/2+1;
		}

		defrag(part);

		for (i=0; i < disk_sz; i++)
			if (disk[i])
				ans[part] += i * (disk[i]-1);
	}

	printf("09: %"PRIu64" %"PRIu64"\n", ans[0], ans[1]);
	return 0;
}
