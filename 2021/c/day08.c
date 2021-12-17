#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compat.h"

/*
 * Approach: instead of implementing a generic solver, sort the input
 * words by criteria that leave them in a known unqiue order.
 *
 * Given input:
 *
 *   acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab 
 *
 * First, sort/group by length:
 *
 *   ab|dab|eafb|cdfbe gcdfa fbcad|cefabd cdfgeb cagedb|acedgfb
 *
 * This leaves the two len=5 and len=6 groups in unknown order.
 * Counting the intersection of those words with words[1] and words[2]
 * results in unique combinations:
 *
 *   ab|dab|eafb|cdfbe gcdfa fbcad|cefabd cdfgeb cagedb|acedgfb
 *    "dab":       2     2     3  |   3      2      3
 *    "eafb":      3     2     3  |   4      3      3
 *    sum:         5     4     6  |   7      5      6
 *
 * Sorting the groups internally by that sum leaves the digits in
 * a known, unique order, mapped by table[].
 */

static const int table[] = {1,7,4,2,5,3,6,0,9,8};
static char *words[10];

static int
sintersect(const char *s1, const char *s2)
{
	int count=0;
	for (; *s1; s1++)
		count += strchr(s2, *s1) != NULL;
	return count;
}

static int
cmp_char(const void *p1, const void *p2)
{
	return *(char*)p1 - *(char*)p2;
}

static int
cmp_len(const void *p1, const void *p2)
{
	return (int)(strlen(*(char**)p1) - strlen(*(char**)p2));
}

static int
cmp_magic(const void *p1, const void *p2)
{
	return
	    sintersect(*(char**)p1, words[1]) +
	    sintersect(*(char**)p1, words[2]) - 
	    sintersect(*(char**)p2, words[1]) -
	    sintersect(*(char**)p2, words[2]);
}

int
main()
{
	char line[100], *lf,*rest,*word;
	int p1=0,p2=0, acc,i;

	while ((rest = fgets(line, 100, stdin))) {
		if ((lf = strchr(rest, '\n')))
			*lf = '\0';
		for (i=0; i<10; i++) {
			if (!(words[i] = strsep(&rest, " ")))
				errx(1, "too few words");
			qsort(words[i], strlen(words[i]), 1, cmp_char);
		}
		if (!strsep(&rest, " "))
			errx(1, "missing separator");

		qsort(words, 10, sizeof(*words), cmp_len);
		qsort(words+3, 3, sizeof(*words), cmp_magic);
		qsort(words+6, 3, sizeof(*words), cmp_magic);

		acc = 0;
		while ((word = strsep(&rest, " "))) {
			qsort(word, strlen(word), 1, cmp_char);
			for (i=0; i<10 && strcmp(words[i], word); i++)
				;
			p1 += table[i]==1 || table[i]==4 ||
			      table[i]==7 || table[i]==8;
			acc = acc*10 + table[i];
		}
		p2 += acc;
	}

	printf("08: %d %d\n", p1, p2);
	return 0;
}
