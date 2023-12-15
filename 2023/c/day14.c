#include "common.h"

static void vis14_begin(void);
static void vis14_emit(void);
static void vis14_end(void);

static char g[101][102];
static int w,h;

static int
hash_grid(void)
{
	int acc=0, x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		acc = acc*31 + g[y][x];

	return acc;
}

static int
score_grid(void)
{
	int acc=0, x,y;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++)
		if (g[y][x]=='O')
			acc += h-y;

	return acc;
}

static void
shift_north(void)
{
	int x,i,j;

	/*
	 * Walk two cursors i and j through each column x. The i cursor
	 * looks for the first . where an O can go. The j cursor looks
	 * ahead for O's. When j finds a # we start again beyond it.
	 */

	for (x=0; x<w; x++)
	for (i=0, j=1; i<h && j<h; )
		if (j <= i) j = i+1;
		else if (g[j][x] == '#') i = j+1;
		else if (g[j][x] != 'O') j++;
		else if (g[i][x] != '.') i++;
		else { g[i++][x] = 'O'; g[j++][x] = '.'; vis14_emit(); }
}

static void
shift_all(void)
{
	int x,y,i,j;

	shift_north();
	
	for (y=0; y<w; y++)
	for (i=0, j=1; i<w && j<w; )
		if (j <= i) j = i+1;
		else if (g[y][j] == '#') i = j+1;
		else if (g[y][j] != 'O') j++;
		else if (g[y][i] != '.') i++;
		else { g[y][i++] = 'O'; g[y][j++] = '.'; vis14_emit(); }
	
	for (x=0; x<w; x++)
	for (i=h-1, j=h-2; i>=0 && j>=0; )
		if (j >= i) j = i-1;
		else if (g[j][x] == '#') i = j-1;
		else if (g[j][x] != 'O') j--;
		else if (g[i][x] != '.') i--;
		else { g[i--][x] = 'O'; g[j--][x] = '.'; vis14_emit(); }

	for (y=0; y<w; y++)
	for (i=w-1, j=w-2; i>=0 && j>=0; )
		if (j >= i) j = i-1;
		else if (g[y][j] == '#') i = j-1;
		else if (g[y][j] != 'O') j--;
		else if (g[y][i] != '.') i--;
		else { g[y][i--] = 'O'; g[y][j--] = '.'; vis14_emit(); }
}

int
main(int argc, char **argv)
{
	static int hist[256];	/* hashes */
	int p1,p2, nhist=0,period=0, nleft, i;

	if (argc > 1)
		DISCARD(freopen(argv[1], "r", stdin));
	
	for (h=0; fgets(g[h], LEN(*g), stdin); h++)
		assert(h+1 < (int)LEN(g));
	
	assert(h>0); w = strlen(g[0])-1;
	assert(w>0);

	vis14_begin();
	vis14_emit();

	shift_north();
	p1 = score_grid();

	for (nleft = 1*1000*1000*1000; nleft; nleft--) {
		shift_all();

		if (!period) {
			assert(nhist < (int)LEN(hist));
			hist[nhist++] = hash_grid();

			for (i=0; i < nhist-1; i++)
				if (hist[i] == hist[nhist-1]) {
					period = nhist-1 - i;
					nleft = nleft % period;
					break;
				}
		}
	}

	vis14_end();
	p2 = score_grid();

	printf("14: %d %d\n", p1, p2);
	return 0;
}

#ifdef WITH_VIS
# include <unistd.h>
# include "vis.h"
# define SCALE	4
# define FRSKIP	5
# define NFRAME	3720

static struct vis vis;
static struct vis_grid vis_grid = {
	.data = (uint8_t *)g,
	.colors = {
		['#'] = {192, 64, 64},
		['O'] = { 64,196, 64}
	},
	.w = LEN(*g),
	.h = LEN(g),
	.cell_sz = SCALE
};

static void
vis14_begin(void)
{
	vis_begin(&vis, "day14.mp4", 60,
	    SCALE * vis_grid.w,
	    SCALE * vis_grid.h);
}

static void
vis14_emit(void)
{
	static int next, frame;

	if (!next-- && frame++ < NFRAME) {
		next = FRSKIP;
		vis_draw_grid(&vis, &vis_grid);
		vis_emit(&vis, 1);
	}
}

static void
vis14_end(void)
{
	vis_emit(&vis, 30);
	vis_end(&vis);
}
#else
static void vis14_begin(void) {}
static void vis14_emit(void) {}
static void vis14_end(void) {}
#endif
