#include "common.h"

struct sig {
	int8_t val;
	uint16_t mod;
	uint8_t slot;
};

struct mod {
	char type, name[12];
	int8_t in_vals[16];	/* '%' only */
	int8_t out_val;		/* '&' only */
	uint16_t out_mod[8];
	uint8_t out_slot[8];
	uint8_t nin, nout;
};

static struct mod mods[32*32], *broadcaster;
static struct sig sigs[128];	/* ring buffer */
static size_t nmods, nsigs, sig0;

static struct mod *
get_mod(const char *name)
{
	struct mod *mod;
	size_t i;

	for (i=0; i<nmods; i++)
		if (!strcmp(mods[i].name, name))
			return &mods[i];

	assert(nmods < LEN(mods));
	mod = &mods[nmods++];
	snprintf(mod->name, sizeof(mod->name), "%s", name);

	return mod;
}

static void
parse(void)
{
	static char line[64];
	struct mod *mod, *dst;
	char *rest, *tok, type, *name, *p;

	while ((rest = fgets(line, sizeof(line), stdin))) {
		tok = strsep(&rest, " ");
		assert(tok);

		if (!strcmp(tok, "broadcaster")) {
			type = '*';
			name = tok;
		} else {
			type = tok[0];
			name = &tok[1];
			assert(type == '%' || type == '&');
			assert(name[0]);
		}

		mod = get_mod(name);
		mod->type = type;

		tok = strsep(&rest, " ");
		assert(!strcmp(tok, "->"));

		while ((tok = strsep(&rest, " "))) {
			if ((p = strchr(tok, ','))) *p = '\0';
			if ((p = strchr(tok, '\n'))) *p = '\0';

			dst = get_mod(tok);

			assert(mod->nout < LEN(mod->out_mod));
			assert(dst->nin < LEN(dst->in_vals));

			mod->out_mod[mod->nout] = (int)(dst - mods);
			mod->out_slot[mod->nout] = dst->nin++;
			mod->nout++;
		}
	}

	broadcaster = get_mod("broadcaster");
	assert(broadcaster->nout);
}

static void
emit_sigs(struct mod *mod, int8_t val)
{
	struct sig *sig;
	size_t i;

	assert(nsigs + mod->nout < LEN(sigs));

	mod->out_val = val;

	for (i=0; i < mod->nout; i++) {
		sig = &sigs[(sig0 + nsigs++) % LEN(sigs)];
		sig->val = val;
		sig->mod = mod->out_mod[i];
		sig->slot = mod->out_slot[i];
	}
}

static int
handle_sig(int *nlo, int *nhi)
{
	struct sig *sig;
	struct mod *mod;
	size_t non, i;

	if (!nsigs)
		return 0;

	sig = &sigs[sig0 % LEN(sigs)];	/* ring buffer */
	sig->val ? (*nhi)++ : (*nlo)++;

	mod = &mods[sig->mod];
	mod->in_vals[sig->slot] = sig->val;

	switch (mod->type) {
	case '%':
		if (!sig->val)
			emit_sigs(mod, (int8_t)!mod->out_val);
		break;

	case '&':
		for (i=0, non=0; i < mod->nin; i++)
			non += mod->in_vals[i] == 1;
		emit_sigs(mod, (int8_t)(non < mod->nin));
		break;
	}

	sig0++;
	nsigs--;

	return 1;
}

static struct mod *
next_of_type(struct mod *mod, char type)
{
	int i;

	assert(mod);

	for (i=0; i < mod->nout; i++)
		if (mods[mod->out_mod[i]].type == type)
			return &mods[mod->out_mod[i]];

	return 0;
}

/*
 * The input has a particular structure: the broadcaster connects to
 * 4 subcircuits that are a chain of flip-flops, some of which connect
 * to a NAND gate.
 *
 *   broadcaster -> %a>%b>%c>%d..
 *                   v     v  v
 *                      &foo     -> ..
 *
 * Since each flip-flop in series has double the period of the one
 * before, they function like a binary counter. In the example above,
 * %a fires every step, %b every 2 steps, %c every 4 and %d every 8.
 *
 * The connections of the flip-flops to the NAND essentially encode
 * a target value for the counter: in the example above, binary 1101
 * (note the order, LSB first vs. MSB first).
 *
 * The NAND gates are connected back to some of the flip flops so that
 * they all reset to 0, so the counter is reset and the period remains
 * stable.
 *
 * finally the sub-circuit NAND gates connect to the final gate through
 * another NAND gate and one serving as an inverter:
 *
 *   &foo \
 *   &bar -> &z -> &inv -> %rx
 *   &baz /
 *
 * So as soon as all NAND gates fire at the same time a HIGH is sent
 * to &inv which sends a LOW to %rx, the end state for part 2.
 *
 * The final question then is when do all sub-circuit NAND gates fire at
 * the same time? That's when their cycles line up, which is their LCM.
 * Those cycles conveniently happen to be prime in the input, so we can
 * just multiply them.
 */
static int64_t
part2(void)
{
	struct mod *bc, *cur;
	int64_t acc=1, period;
	int i, pos;

	bc = get_mod("broadcaster");

	for (i=0; i < bc->nout; i++) {
		period = pos = 0;
		cur = &mods[bc->out_mod[i]];

		do {
			period |= !!next_of_type(cur, '&') << (pos++);
		} while ((cur = next_of_type(cur, '%')));

		acc *= period;
	}

	return acc;
}

int
main(int argc, char **argv)
{
	int64_t p2;
	int nlo=0,nhi=0, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	parse();

	for (i=0; i < 1000; i++) {
		nlo++; /* the button press */
		emit_sigs(broadcaster, 0);

		while (handle_sig(&nlo, &nhi))
			;
	}

	p2 = part2();

	printf("20: %d %"PRId64"\n", nlo*nhi, p2);
	return 0;
}
