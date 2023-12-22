#include "common.h"

/*
 * Yes, it's a hash table. Did I pick a language with built in hash
 * tables? Of course I didn't. Could I have used one of the many
 * libraries implementing one? Sure. But the real question is, can
 * we make do with stuffing things into a few static arrays at nearly
 * zero memory and runtime cost? Yes!
 */

struct slot { char label[8]; int lens; };
struct box { struct slot slots[8]; int nslots; };

static struct box boxes[256];

static int
hasher(const char *s)
{
	int acc=0;
	for (; *s; s++) acc = (acc + *s) * 17 % 256;
	return acc;
}

static void
eval(const char *label, int op, int lens)
{
	struct box *box;
	int hash, i;

	hash = hasher(label);
	box = &boxes[hash];
	assert(hash >= 0 && hash < (int)LEN(boxes));

	switch (op) {
	case '-':
		for (i=0; i < box->nslots; i++) {
			if (strcmp(label, box->slots[i].label))
				continue;
			memmove(
			    &box->slots[i],
			    &box->slots[i+1],
			    sizeof(*box->slots) * (box->nslots-i-1));
			box->nslots--;
			break;
		}
		break;
	case '=':
		for (i=0; i < box->nslots; i++)
			if (!strcmp(label, box->slots[i].label)) {
				box->slots[i].lens = lens;
				break;
			}
		if (i == box->nslots) {
			assert(i+1 < (int)LEN(box->slots));
			snprintf(
			    box->slots[i].label,
			    sizeof(box->slots[i].label),
			    "%.7s", label);
			box->slots[i].lens = lens;
			box->nslots++;
		}
		break;
	default:
		assert(!"bad op");
	}
}

int
main(int argc, char **argv)
{
	char ins[9];
	int p1=0,p2=0, len=0, op, c, i,j;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	while (!feof(stdin))
		if ((c = getchar()) == ',' || c == EOF) {
			ins[len] = '\0';
			p1 += hasher(ins);
			for (i=0; isalpha(ins[i]); i++) ;
			assert(ins[i]);
			op = ins[i];	/* save the operator */
			ins[i] = '\0';	/* terminate label */
			eval(ins, op, atoi(ins+i+1));
			len = 0;
		} else if (!isspace(c)) {
			assert(len+1 < (int)LEN(ins));
			ins[len++] = c;
		}

	for (i=0; i < (int)LEN(boxes); i++)
	for (j=0; j < boxes[i].nslots; j++)
		p2 += (i+1) * (j+1) * boxes[i].slots[j].lens;

	printf("15: %d %d\n", p1, p2);
	return 0;
}
