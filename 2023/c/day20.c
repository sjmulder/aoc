#include "common.h"

struct sig {
	int8_t val;
	uint16_t mod;
	uint8_t slot;
};

struct mod {
	char type, name[3];
	int8_t in_vals[16];	/* '%' only */
	int8_t out_val;		/* '&' only */
	uint16_t out_mod[8];
	uint8_t out_slot[8];
	uint8_t nin, nout;
};

static struct mod mods[32*32], *mod_br;
static struct sig sigs[128];	/* ring buffer */
static size_t nsigs, sig0;

static struct mod *
get_mod(const char *name)
{
	struct mod *mod;
	uint16_t id;

	assert(name[0]);
	id = (uint16_t)name[0] %32 *32 + (uint16_t)name[1] %32;
	assert(id < LEN(mods));

	mod = &mods[id];
	if (!mod->name[0])
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

			mod->out_mod[mod->nout] = dst - mods;
			mod->out_slot[mod->nout] = dst->nin++;
			mod->nout++;
		}
	}

	mod_br = get_mod("br"); assert(mod_br->nout);
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
			emit_sigs(mod, !mod->out_val);
		break;

	case '&':
		for (i=0, non=0; i < mod->nin; i++)
			non += mod->in_vals[i] == 1;
		emit_sigs(mod, non < mod->nin);
		break;
	}

	sig0++;
	nsigs--;

	return 1;
}

int
main(int argc, char **argv)
{
	int nlo=0,nhi=0, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));

	parse();

	for (i=0; i < 1000; i++) {
		nlo++; /* the button press */
		emit_sigs(mod_br, 0);

		while (handle_sig(&nlo, &nhi))
			;
	}

	printf("20: %d <TODO>\n", nlo*nhi);
	return 0;
}
