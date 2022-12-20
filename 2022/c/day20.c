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
static int64_t val[SZ];	/* values by id */
static int ix[SZ];	/* indexes by id */
static int id[SZ];	/* id by index */
static int n, zero_id;

static int64_t
run(int steps)
{
	int s,i,j, f,t;
	int64_t v;

	for (i=0; i<n; i++) ix[i] = i;
	for (i=0; i<n; i++) id[i] = i;

	for (s=0; s<steps; s++)
	for (i=0; i<n; i++) {
		v = val[i];
		f = ix[i];	/* f: from index, t: to index */
		t = wrap(f + (int)(v%(n-1)), n);

		if (v>0 && t>f) {
			memmove(id+f, id+f+1, (t-f)*sizeof(*id));
			id[t] = i;
			for (j=f; j<=t; j++) ix[id[j]] = j;
		} else if (v<0 && t<f) {
			memmove(id+t+1, id+t, (f-t)*sizeof(*id));
			id[t] = i;
			for (j=t; j<=f; j++) ix[id[j]] = j;
		} else if (v>0 && t<f) {
			memmove(id+f, id+f+1, (n-f-1)*sizeof(*id));
			id[n-1] = id[0];
			memmove(id, id+1, t*sizeof(*id));
			id[t] = i;
			for (j=0; j<=t; j++) ix[id[j]] = j;
			for (j=f; j<n;  j++) ix[id[j]] = j;
		} else if (v<0 && t>f) {
			memmove(id+1, id, f*sizeof(*id));
			id[0] = id[n-1];
			memmove(id+t+1, id+t, (n-t-1)*sizeof(*id));
			id[t] = i;
			for (j=0; j<=f; j++) ix[id[j]] = j;
			for (j=t; j<n;  j++) ix[id[j]] = j;
		}
	}

	return
	    val[id[wrap(ix[zero_id] + 1000, n)]] +
	    val[id[wrap(ix[zero_id] + 2000, n)]] +
	    val[id[wrap(ix[zero_id] + 3000, n)]];
}

int
main()
{
	int i;
	int64_t p1,p2;

	while (scanf("%" PRId64, &val[n]) == 1) {
		if (val[n] == 0) zero_id = n;
		n++;
		assert(n < SZ);
	}

	p1 = run(1);
	for (i=0; i<n; i++) val[i] *= 811589153;
	p2 = run(10);

	printf("20: %" PRId64 " %" PRId64 "\n", p1, p2);
	return 0;
}
