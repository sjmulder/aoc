#include <stdio.h>
#include <string.h>
#include <err.h>
#include <intcode.h>

#define LEN(a) (sizeof((a))/sizeof((a)[0]))

#define LOG(...) ((void)0)
/*#define LOG(...) fprintf(stderr, __VA_ARGS__)*/

typedef struct amp {
	Icvm vm;
	int phase;
	int input, output;
	int flags;
#define AMP_PHASEREAD 0x1
#define AMP_PAUSED 0x2
} Amp;

static int in_cb(void *);
static void out_cb(int, void *);

int
main()
{
	static Icvm image;
	static Amp amps[5]; 

	int a,i,j,k,l,m;
	int val = 0, max = 0;

	ic_load(&image, stdin);
	image.in_cb = in_cb;
	image.out_cb = out_cb;

	for (i = 5; i < 10; i++)
	for (j = 5; j < 10; j++)
	for (k = 5; k < 10; k++)
	for (l = 5; l < 10; l++)
	for (m = 5; m < 10; m++) {
		if (i == m || i == l || i == k || i == j  ||
		    j == m || j == l || j == k ||
		    k == m || k == l ||
		    l == m)
			continue;

		LOG("%d %d %d %d %d\n", i, j, k, l, m);

		for (a = 0; a < (int)LEN(amps); a++) {
			memcpy(&amps[a].vm, &image, sizeof(amps[a].vm));
			amps[a].vm.user = &amps[a];
			amps[a].flags = 0;
		}

		amps[0].input = 0;

		amps[0].phase = i;
		amps[1].phase = j;
		amps[2].phase = k;
		amps[3].phase = l;
		amps[4].phase = m;

		a = 0;
		while (1) {
			ic_step(&amps[a].vm, NULL);

			if (amps[a].vm.flags & IC_HALTED) {
				val = amps[LEN(amps)-1].output;
				break;
			}

			if (amps[a].flags & AMP_PAUSED) {
				val = amps[a].output;
				amps[a].flags &= ~AMP_PAUSED;
				a = (a+1) % LEN(amps);
				LOG(" %12d -> amp %d\n", val, a);
				amps[a].input = val;
			}
		}

		LOG(" %d\n", val);
		if (val > max)
			max = val;
	}

	printf("%d\n", max);
	return 0;
}

static int
in_cb(void *user)
{
	Amp *amp;

	amp = user;
	if (amp->flags & AMP_PHASEREAD)
		return amp->input;
	else {
		amp->flags |= AMP_PHASEREAD;
		return amp->phase;
	}
}

static void
out_cb(int val, void *user)
{
	Amp *amp;

	amp = user;
	amp->output = val;
	amp->flags |= AMP_PAUSED;
}
