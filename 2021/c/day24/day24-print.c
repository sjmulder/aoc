#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int64_t monad(int input[14]);

int
main()
{
	int input[14], i,j;
	int64_t n;
	
	for (i=0; i<14; i++)
		input[i] = 9;

#if 0
	//for (input[0] = 9; input[0] > 0; input[0]--)
	//for (input[1] = 9; input[1] > 0; input[1]--)
	//for (input[2] = 9; input[2] > 0; input[2]--)
	for (input[3] = 9; input[3] > 0; input[3]--)
	for (input[4] = 9; input[4] > 0; input[4]--)
	for (input[5] = 9; input[5] > 0; input[5]--)
	//for (input[6] = 9; input[6] > 0; input[6]--)
	//for (input[7] = 9; input[7] > 0; input[7]--)
	//for (input[8] = 9; input[8] > 0; input[8]--)
	//for (input[9] = 9; input[9] > 0; input[9]--)
	//for (input[10] = 9; input[10] > 0; input[10]--)
	//for (input[11] = 9; input[11] > 0; input[11]--)
	//for (input[12] = 9; input[12] > 0; input[12]--)
	//for (input[13] = 9; input[13] > 0; input[13]--)
#if 1
	{
		for (i=0; i<14; i++)
			putchar('0' + input[i]);

		n = monad(input);

		//printf(" %"PRId64"\n", monad(input));

		printf(
		    " %2"PRId64" %2"PRId64" %2"PRId64" %2"PRId64
		    " %2"PRId64" %2"PRId64"\n",
		    n/26/26/26/26/26,
		    n/26/26/26/26 %26,
		    n/26/26/26 %26,
		    n/26/26 %26,
		    n/26 %26,
		    n %26);
	}

	return 0;
}
