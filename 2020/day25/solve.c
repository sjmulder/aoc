#include <stdio.h>
#include "../compat/stdint.h"
#include "../compat/inttypes.h"

#define CARD_PUB 6930903  /* sample: 5764801 */
#define DOOR_PUB 19716708 /* sample: 17807724 */

int
main()
{
	uint64_t pub=1, key1=1, key2=1, key=0;

	/* credit to /u/ramuuns-u for checking both pub keys at once */
	while (1) {
		pub  = pub*7 % 20201227;
		key1 = key1*CARD_PUB % 20201227;
		key2 = key1*DOOR_PUB % 20201227;
		if (pub == CARD_PUB) {key=key2; break;}
		if (pub == DOOR_PUB) {key=key1; break;}
	}

	printf("%" PRIu64 "\n", key);
	//getchar();
	return 0;
}
