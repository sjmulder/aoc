#include "common.h"

int
main(int argc, char **argv)
{
	char line[256], *rest, *sid, *sround, *scolor;
	int p1=0, p2=0, id;
	int nred, ngreen, nblue;
	int mred, mgreen, mblue;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	while ((rest = fgets(line, sizeof(line), stdin))) {
		sid = strsep(&rest, ":");
		sscanf(sid, "Game %d", &id);
		mred = mblue = mgreen = 0;

		while ((sround = strsep(&rest, ";"))) {
			nred = nblue = ngreen = 0;

			while ((scolor = strsep(&sround, ",")))
				if (strstr(scolor, "red"))
					nred = atoi(scolor);
				else if (strstr(scolor, "green"))
					ngreen = atoi(scolor);
				else if (strstr(scolor, "blue"))
					nblue = atoi(scolor);
			
			if (nred > mred) mred = nred;
			if (ngreen > mgreen) mgreen = ngreen;
			if (nblue > mblue) mblue = nblue;
		}
	
		if (mred <= 12 && mgreen <= 13 && mblue <= 14)
			p1 += id;

		p2 += mred * mgreen * mblue;
	}

	printf("%d %d\n", p1, p2);
	return 0;
}
