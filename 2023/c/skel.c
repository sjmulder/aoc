#include "common.h"

int
main(int argc, char **argv)
{
	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	return 0;
}
