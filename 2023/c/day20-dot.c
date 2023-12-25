#include "common.h"

int
main(int argc, char **argv)
{
	static char line[64];
	char *rest, *tok, *name, *p;
	int i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	printf("digraph day20 {\n");

	while ((rest = fgets(line, sizeof(line), stdin))) {
		tok = strsep(&rest, " ");
		assert(tok);

		name = tok;
		name += !isalpha(name[0]);

		if (tok[0] == '&')
			printf("  \"%s\" [shape=box]\n", name);

		tok = strsep(&rest, " ");
		assert(!strcmp(tok, "->"));

		for (i=0; (tok = strsep(&rest, " ")); i++) {
			if ((p = strchr(tok, ','))) *p = '\0';
			if ((p = strchr(tok, '\n'))) *p = '\0';

			printf(
			    "  \"%s\"-> \"%s\" [label=\"%d\"]\n",
			    name, tok, i);
		}
	}

	printf("}\n");
	return 0;
}
