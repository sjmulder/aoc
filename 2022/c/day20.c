#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#include <assert.h>

#ifdef WITH_VIS
# include "vis.h"
#endif

static void vis20_begin(void);
static void vis20_emit(int y);
static void vis20_end(void);

#define SZ	5120
#define KEY	811589153

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
			if (v>0) t++;	/* shifts with array */
			memmove(id+t+1, id+t, (f-t)*sizeof(*id));
		}

		id[t] = i;
		vis20_emit(i);
	}

	for (i=0; id[i] != zero_id; i++) ;

	return
	    val[id[(i+1000)%n]] +
	    val[id[(i+2000)%n]] +
	    val[id[(i+3000)%n]];
}

int
main(int argc, char **argv)
{
	int i;
	int64_t p1,p2;

	if (argc > 1)
		freopen(argv[1], "r", stdin);

	while (scanf("%" PRId64, &val[n]) == 1) {
		if (val[n] == 0) zero_id = n;
		n++;
		assert(n < SZ);
	}

	vis20_begin();

	p1 = run(1);
	for (i=0; i<n; i++) val[i] *= KEY;
	p2 = run(10);

	vis20_end();
	printf("20: %" PRId64 " %" PRId64 "\n", p1, p2);
	return 0;
}

/* Visualisation stuff from here on */

#ifdef WITH_VIS
# define SCALE	5
# define SPEED	3

static struct vis vis;
static int prevy;

static int max(int a, int b) { return a>b ? a : b; }

static void
vis20_begin(void)
{
	vis_begin(&vis, "day20.mp4", 60, n/SCALE, n/SCALE);
}

static void
vis20_emit(int y)
{
	int x,i, v, r,g,b;
	uint8_t *pix;

	for (x=0; x < n/SCALE; x++) {
		r = g = b = 0;
		for (i=0; i<SCALE; i++) {
			v = id[x*SCALE+i] % (255*3);
			r += max(0, 255-abs(v-255*3)) + max(0, 255-v);
			g += max(0, 255-abs(v-255*1));
			b += max(0, 255-abs(v-255*2));
		}

		pix = vis.frame + (y/SCALE)*(n/SCALE)*3 + x*3;
		if (y/SCALE != prevy/SCALE)
			memset(pix, 0, sizeof(*pix *3));
		pix[0] += r/SCALE/SCALE;
		pix[1] += g/SCALE/SCALE;
		pix[2] += b/SCALE/SCALE;
	}

	prevy = y;

	if (y % (SCALE*SPEED) == (SCALE*SPEED)-1)
		vis_emit(&vis, 1);
}

static void
vis20_end(void)
{
	vis_end(&vis);
}
#else
static void vis20_begin(void) {}
static void vis20_emit(int y) { (void)y; }
static void vis20_end(void) {}
#endif
