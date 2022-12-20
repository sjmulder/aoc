#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#include <assert.h>

#define SZ	5120

static int sgn(int64_t x) { return x<0 ? -1 : x>0 ? 1 : 0; }
static int wrap(int i, int n) { return (i %n +n) %n; }

/*
 * id:  the 'identity' of the number (its original index)
 * idx: index in the output array
 */
static int64_t id_val[SZ];
static int id_idx[SZ];
static int idx_id[SZ];
static int n, zero_id;

static int64_t
run(int steps)
{
	int s, i,j;
	int cur_idx, new_idx, d;
	int a_id, b_id, a_idx, b_idx;
	int64_t val;

	for (i=0; i<n; i++) id_idx[i] = i;
	for (i=0; i<n; i++) idx_id[i] = i;

	for (s=0; s<steps; s++)
	for (i=0; i<n; i++) {
		val = id_val[i];
		cur_idx = id_idx[i];
		new_idx = cur_idx + (int)(val % (n-1));
		d = sgn(val);

		for (j = cur_idx; j != new_idx; j += d) {
			a_id = idx_id[(a_idx = wrap(j, n))];
			b_id = idx_id[(b_idx = wrap(j+d, n))];

			id_idx[a_id] = b_idx; idx_id[a_idx] = b_id;
			id_idx[b_id] = a_idx; idx_id[b_idx] = a_id;
		}
	}

	return
	    id_val[idx_id[wrap(id_idx[zero_id] + 1000, n)]] +
	    id_val[idx_id[wrap(id_idx[zero_id] + 2000, n)]] +
	    id_val[idx_id[wrap(id_idx[zero_id] + 3000, n)]];
}

int
main()
{
	int i;
	int64_t p1,p2;

	while (scanf("%" PRId64, &id_val[n]) == 1) {
		if (id_val[n] == 0)
			zero_id = n;
		n++;
		assert(n < SZ);
	}

	p1 = run(1);
	for (i=0; i<n; i++) id_val[i] *= 811589153;
	p2 = run(10);

	printf("20: %" PRId64 " %" PRId64 "\n", p1, p2);
	return 0;
}
