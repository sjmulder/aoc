#define _CRT_SECURE_NO_WARNINGS
#define BUFSZ    64
#define LISTSZ  256
#define BLOCKSZ  16
#define NBLOCKS (LISTSZ/BLOCKSZ)
#define NROUNDS  64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const unsigned char salt[] = {17, 31, 73, 47, 23};

static void
initlist(unsigned char *list)
{
	int i;

	for (i=0; i<LISTSZ; i++)
		list[i] = i;
}

static void
readinput(unsigned char *buf, unsigned char **input)
{
	unsigned char *end;

	end = buf + fread(buf, 1, BUFSZ-1, stdin)-1;

	if (ferror(stdin)) {
		perror(NULL);
		exit(1);
	}

	if (!feof(stdin)) {
		fputs("input buffer too small\n", stderr);
		exit(1);
	}

	*input = buf;
	while (*input < buf && isspace(**input))
		(*input)++;
	while (end > *input && isspace(*end))
		end--;
	end[1] = '\0';
}

static void
apply(const unsigned char *str, int *pos, int *skip, unsigned char *list)
{
	int i;
	unsigned char *p, *q, temp;

	for (; *str; str++) {
		for (i=0; i < (*str)/2; i++) {
			p = list + (*pos+i) % LISTSZ;
			q = list + (*pos+*str-i-1) % LISTSZ;
			temp = *p; *p = *q; *q = temp;
		}

		*pos += *str + (*skip)++;
	}
}

static void
tohash(unsigned char *list, unsigned char *hash)
{
	int block, i;

	memset(hash, 0, NBLOCKS);
	for (block=0; block<NBLOCKS; block++)
		for (i=0; i<BLOCKSZ; i++)
			hash[block] ^= list[block*BLOCKSZ+i];
}

static void
printhash(unsigned char *hash)
{
	int i;

	for (i=0; i<NBLOCKS; i++)
		printf("%02x", hash[i]);
	putchar('\n');
}

int
main()
{
	unsigned char buf[BUFSZ], *input;
	unsigned char list[LISTSZ], hash[NBLOCKS];
	int pos=0, skip=0, i;

	initlist(list);	
	readinput(buf, &input);

	for (i=0; i<NROUNDS; i++) {
		apply(input, &pos, &skip, list);
		apply(salt, &pos, &skip, list);
	}

	tohash(list, hash);
	printhash(hash);
	return 0;
}
