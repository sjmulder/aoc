#include <stdio.h>
#include <ctype.h>

static int
recur(char end)
{
	int c=0,n=0, sign=1, acc=0;
	char *match = "\"red\"";

	while (c != end && c != EOF) {
		c = getchar();

		if (end != '}' || c != *match)
			match = "\"red\"";
		else if (!*(++match)) {
			while ((c = getchar()) != EOF)
				if (c == end) break;
				else if (c == '{') recur('}');
				else if (c == '[') recur(']');
			return 0;
		}

		if (isdigit(c))    n = n*10 + (c-'0');
		else if (c == '[') acc += recur(']');
		else if (c == '{') acc += recur('}');
		else {
			acc += sign * n;
			sign = c=='-' ? -1 : 1;
			n = 0;
		}
	}

	return acc;
}

int main() { printf("%d\n", recur(-1)); return 0; }
