#include <stdio.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

#define CARD_PUB 6930903  /* sample: 5764801 */
#define DOOR_PUB 19716708 /* sample: 17807724 */

int
main()
{
	uint64_t pub=1, key=1;

	while (pub != CARD_PUB) {
		pub = pub*7 % 20201227;
		key = key*DOOR_PUB % 20201227;
	}

	printf("%" PRIu64 "\n", key);
	//getchar();
	return 0;
}
