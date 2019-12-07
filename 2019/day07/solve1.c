#include <stdio.h>
#include <string.h>
#include <err.h>
#include <intcode.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

typedef struct amp {
	int in[2], in_i;
	int out;
} Amp;

static int run_amp(Icvm *, int, int);
static int in_cb(void *);
static void out_cb(int, void *);

int
main()
{
	static Icvm vm;
	int i,j,k,l,m;
	int val, max = 0;

	ic_load(&vm, stdin);
	vm.in_cb = in_cb;
	vm.out_cb = out_cb;

	for (i = 0; i < 5; i++)
	for (j = 0; j < 5; j++)
	for (k = 0; k < 5; k++)
	for (l = 0; l < 5; l++)
	for (m = 0; m < 5; m++) {
		if (i == m || i == l || i == k || i == j  ||
		    j == m || j == l || j == k ||
		    k == m || k == l ||
		    l == m)
			continue;

		val = run_amp(&vm, i, 0);
		val = run_amp(&vm, j, val);
		val = run_amp(&vm, k, val);
		val = run_amp(&vm, l, val);
		val = run_amp(&vm, m, val);

		if (val > max)
			max = val;
	}

	printf("%d\n", max);
	return 0;
}

static int
run_amp(Icvm *vm, int phase, int input)
{
	static Icvm copy;
	Amp amp;

	memset(&amp, 0, sizeof(amp));
	amp.in[0] = phase;
	amp.in[1] = input;

	memcpy(&copy, vm, sizeof(copy));
	copy.user = &amp;

	ic_run(&copy, NULL);

	return amp.out;
}

static int
in_cb(void *user)
{
	Amp *amp;

	amp = user;
	if (amp->in_i >= (int)LEN(amp->in))
		errx(1, "out of inputs");

	return amp->in[amp->in_i++];
}

static void
out_cb(int val, void *user)
{
	Amp *amp;

	amp = user;
	amp->out = val;
}
