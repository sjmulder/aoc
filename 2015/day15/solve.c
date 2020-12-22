#include <stdio.h>
#include <err.h>

#define CAP	5
#define NPROP	4	/* excl cal */
#define NSPOON	100

int
main()
{
	static int props[CAP][NPROP], cals[CAP], spoon[CAP+1];
	int i,j, len=0, best=0,best500=0, cal,prod,sum,amount;

	while (scanf("%*s capacity %d, durability %d, flavor %d, "
	    "texture %d, calories %d", &props[len][0], &props[len][1],
	    &props[len][2], &props[len][3], &cals[len]) == NPROP+1)
		if (++len >= CAP)
			errx(1, "props overflow");

	spoon[len] = 100;

	while (spoon[0] != NSPOON) {
		cal=0; prod=1;

		for (i=0; i<NPROP; i++) {
			sum = 0;
			for (j=0; j<len; j++) {
				amount = spoon[j+1] - spoon[j];
				sum += amount * props[j][i];
			}
			prod *= sum<0 ? 0 : sum;
			printf("%d ", sum);
		}

		for (i=0; i<len; i++) {
			amount = spoon[i+1] - spoon[i];
			cal += amount * cals[i];
			printf(" %d", amount);
		}

		printf("  %d (%d)\n", prod, cal);

		if (prod > best)
			best = prod;
		if (cal == 500 && prod > best500)
			best500 = prod;

		for (i=len-1; i>=0 && ++spoon[i] == NSPOON+1; i--)
			;
		for (i++; i<len; i++)
			spoon[i] = spoon[i-1];
	}

	printf("%d %d\n", best, best500);
}
