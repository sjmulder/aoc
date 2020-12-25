#include <stdio.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

#define CARD_PUB 6930903  /* sample: 5764801 */
#define DOOR_PUB 19716708 /* sample: 17807724 */

int
main()
{
	int i, card_sec;
	uint64_t val;

	for (val=1, card_sec=0; val != CARD_PUB; card_sec++)
		val = val*7 % 20201227;
	for (val=1, i=0; i<card_sec; i++)
		val = val*DOOR_PUB % 20201227;

	printf("%" PRIu64 "\n", val);
	//getchar();

	return 0;
}
