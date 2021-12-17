#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "compat.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define DUMP 0

struct packet {
	int ver;
	int type;
#define PT_SUM	0
#define PT_PRD	1
#define PT_MIN	2
#define PT_MAX	3
#define PT_LIT	4
#define PT_GT	5
#define PT_LT	6
#define PT_EQ	7
	uint64_t val;
	struct packet *children;
	struct packet *next;
};

struct bitstream {
	FILE *f;
	uint64_t pos;
	uint64_t buf;
	int buflen;
};

static uint64_t
read_uint(struct bitstream *bs, int nbits)
{
	uint64_t c, ret;

	while (bs->buflen < nbits) {
		if (bs->buflen + 4 > 64)
			errx(1, "bit buffer full");
		if (scanf("%1"PRIx64, &c) != 1)
			errx(1, "unexpected char or EOF");

		bs->buflen += 4;
		bs->buf = bs->buf << 4 | c;
	}

	ret = bs->buf >> (bs->buflen - nbits);
	bs->pos += nbits;
	bs->buflen -= nbits;
	bs->buf &= ((uint64_t)1 << bs->buflen) -1;

	return ret;
}

static struct packet *
read_packet(struct bitstream *bs)
{
	struct packet *p, **tailp;
	uint64_t chunk, length, start;
	int count, i;

	if (!(p = malloc(sizeof(*p))))
		err(1, "malloc");

	memset(p, 0, sizeof(*p));
	p->ver = (int)read_uint(bs, 3);
	p->type = (int)read_uint(bs, 3);

	if (p->type == PT_LIT) {
		do {
			chunk = read_uint(bs, 5);
			p->val = p->val << 4 | (chunk & 15);
			p->val |= chunk & 5;
		} while (chunk >> 4);
	} else if (read_uint(bs, 1)) {
		count = (int)read_uint(bs, 11);
		tailp = &p->children;

		for (i=0; i<count; i++)
			tailp = &(*tailp = read_packet(bs))->next;
	} else {
		length = read_uint(bs, 15);
		start = bs->pos;
		tailp = &p->children;

		while (bs->pos < start + length)
			tailp = &(*tailp = read_packet(bs))->next;
	}

	return p;
}

#if DUMP
static void
dump(struct packet *p)
{
	struct packet *child;

	if (p->type == PT_LIT)
		{ printf("%"PRIu64, p->val); return; }

	putchar('(');

	switch (p->type) {
	case PT_SUM: putchar('+'); break;
	case PT_PRD: putchar('*'); break;
	case PT_MIN: printf("min"); break;
	case PT_MAX: printf("max"); break;
	case PT_GT:  putchar('>'); break;
	case PT_LT:  putchar('<'); break;
	case PT_EQ:  putchar('='); break;
	default: errx(1, "bad type: %d\n", p->type);
	}

	for (child = p->children; child; child = child->next)
		{ putchar(' '); dump(child); }

	putchar(')');
}
#endif

static uint64_t
eval(struct packet *p)
{
	struct packet *child;
	uint64_t acc=0;

	if (p->type == PT_LIT) return p->val;
	if (!p->children) return 0;

	acc = eval(p->children);

	for (child = p->children->next; child; child = child->next)
		switch (p->type) {
		case PT_SUM: acc += eval(child); break;
		case PT_PRD: acc *= eval(child); break;
		case PT_GT:  acc = acc > eval(child); break;
		case PT_LT:  acc = acc < eval(child); break;
		case PT_EQ:  acc = acc == eval(child); break;
		case PT_MIN: acc = MIN(acc, eval(child)); break;
		case PT_MAX: acc = MAX(acc, eval(child)); break;
		default: errx(1, "bad type: %d\n", p->type);
		}

	return acc;
}

static int
sum_vers(struct packet *p)
{
	int sum=0;

	sum += p->ver;
	if (p->children) sum += sum_vers(p->children);
	if (p->next) sum += sum_vers(p->next);

	return sum;
}

int
main()
{
	struct bitstream bs;
	struct packet *root;

	memset(&bs, 0, sizeof(bs));
	bs.f = stdin;
	root = read_packet(&bs);

#if DUMP
	dump(root);
	putchar('\n');
#endif

	printf("16: %d %"PRIu64"\n", sum_vers(root), eval(root));
	return 0;
}
