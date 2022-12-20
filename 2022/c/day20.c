#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#include <assert.h>

#define SZ	5120

/*
 * id:  the 'identity' of the number (its original index)
 * idx: index in the output array
 */
static int64_t val[SZ];	/* values by id */
static int id[SZ];	/* id by index */
static int n, zero_id;

static int64_t
run(int steps)
{
	int s,i, f,t;
	int64_t v;

	for (i=0; i<n; i++) id[i] = i;

	for (s=0; s<steps; s++)
	for (i=0; i<n; i++) {
		v = val[i];
		for (f=0; id[f]!=i; f++) ;	/* from idx */
		t = (((f + v%(n-1)) %n)+n)%n;	/* to idx */

		if (t>f) {
			if (v<0) t--;	/* shifts with array */
			memmove(id+f, id+f+1, (t-f)*sizeof(*id));
		} else if (t<f) {
			if (v>0) t++;	/* shift with array */
			memmove(id+t+1, id+t, (f-t)*sizeof(*id));
		}

		id[t] = i;
	}

	for (i=0; id[i] != zero_id; i++) ;

	return
	    val[id[(i+1000)%n]] +
	    val[id[(i+2000)%n]] +
	    val[id[(i+3000)%n]];
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
