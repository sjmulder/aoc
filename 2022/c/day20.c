#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#include <assert.h>

#define SZ	5120

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
	int cur_idx, new_idx;
	int64_t val;

	for (i=0; i<n; i++) id_idx[i] = i;
	for (i=0; i<n; i++) idx_id[i] = i;

	for (s=0; s<steps; s++)
	for (i=0; i<n; i++) {
		val = id_val[i];
		cur_idx = id_idx[i];
		new_idx = wrap(cur_idx + (int)(val % (n-1)), n);

		if (val > 0 && new_idx > cur_idx) {
			memmove(
			    &idx_id[cur_idx],
			    &idx_id[cur_idx+1],
			    (new_idx - cur_idx) * sizeof(*idx_id));

			idx_id[new_idx] = i;

			for (j = cur_idx; j <= new_idx; j++)
				id_idx[idx_id[j]] = j;
		} else if (val < 0 && new_idx < cur_idx) {
			memmove(
			    &idx_id[new_idx+1],
			    &idx_id[new_idx],
			    (cur_idx - new_idx) * sizeof(*idx_id));

			idx_id[new_idx] = i;

			for (j = new_idx; j <= cur_idx; j++)
				id_idx[idx_id[j]] = j;
		} else if (val > 0 && new_idx < cur_idx) {
			memmove(
			    &idx_id[cur_idx],
			    &idx_id[cur_idx+1],
			    (n - cur_idx -1) * sizeof(*idx_id));

			idx_id[n-1] = idx_id[0];

			memmove(
			    &idx_id[0],
			    &idx_id[1],
			    new_idx * sizeof(*idx_id));

			idx_id[new_idx] = i;

			for (j=0; j <= new_idx; j++)
				id_idx[idx_id[j]] = j;
			for (j = cur_idx; j < n; j++)
				id_idx[idx_id[j]] = j;
		} else if (val < 0 && new_idx > cur_idx) {
			memmove(
			    &idx_id[1],
			    &idx_id[0],
			    cur_idx * sizeof(*idx_id));

			idx_id[0] = idx_id[n-1];

			memmove(
			    &idx_id[new_idx+1],
			    &idx_id[new_idx],
			    (n - new_idx -1) * sizeof(*idx_id));

			idx_id[new_idx] = i;

			for (j=0; j <= cur_idx; j++)
				id_idx[idx_id[j]] = j;
			for (j = new_idx; j < n; j++)
				id_idx[idx_id[j]] = j;
		}

		idx_id[new_idx] = i;

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
