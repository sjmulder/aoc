#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "compat/stdint.h"
#include "compat/inttypes.h"

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

/*
 * Dictionary:
 *  tile   - single block
 *  layout - arrangement of tiles (references)
 *  slot   - position in a layout
 *  grid   - rendered layout (edges removed)
 */

struct tile {
	int id;
	char g[10][10];
};

static const char monster[3][21] = {
    "                  # ",
    "#    ##    ##    ###",
    " #  #  #  #  #  #   "
};

static struct tile tiles[144];
static struct tile *layout[144];
static char grid[100][100];
static int ntiles;
static int tilesz;
static int layoutsz;
static int gridsz;

static void
parse(FILE *f)
{
	int id, c, x,y;
	struct tile *tile;

	while (1) {
		if (fscanf(f, "Tile %d:\n", &id) != 1)
			break;

		assert(ntiles < LEN(tiles));

		tile = &tiles[ntiles];
		layout[ntiles] = tile;
		ntiles++;
		x = y = 0;

		while (layoutsz*layoutsz < ntiles)
			layoutsz++;

		while ((c = fgetc(f)) != EOF) {
			if (c == '\n' && !x)
				break;
			if (c == '\n')
				{ x = 0; y++; continue; }

			if (x >= tilesz) tilesz = x+1;
			if (y >= tilesz) tilesz = y+1;

			assert(x < LEN(tile->g[0]));
			assert(y < LEN(tile->g));
			assert(c == '#' || c == '.');

			tile->id = id;
			tile->g[y][x] = c == '#';
			x++;
		}
	}

	gridsz = layoutsz * (tilesz-2);
	assert(gridsz <= LEN(grid));
	assert(gridsz <= LEN(grid[0]));
}

static void
rotate(struct tile *tile)
{
	struct tile orig;
	int x,y;

	memcpy(&orig, tile, sizeof(orig));

	for (y=0; y<tilesz; y++)
	for (x=0; x<tilesz; x++)
		tile->g[x][tilesz-1-y] = orig.g[y][x];
}

static void
rotategrid(void)
{
	char orig[LEN(grid)][LEN(grid[0])];
	int x,y;

	memcpy(orig, grid, sizeof(orig));

	for (y=0; y<gridsz; y++)
	for (x=0; x<gridsz; x++)
		grid[x][gridsz-1-y] = orig[y][x];
}

static void
flip(struct tile *tile)
{
	struct tile orig;
	int x,y;

	memcpy(&orig, tile, sizeof(orig));

	for (y=0; y<tilesz; y++)
	for (x=0; x<tilesz; x++)
		tile->g[y][tilesz-1-x] = orig.g[y][x];
}

static void
flipgrid(void)
{
	char orig[LEN(grid)][LEN(grid[0])];
	int x,y;

	memcpy(orig, grid, sizeof(orig));

	for (y=0; y<gridsz; y++)
	for (x=0; x<gridsz; x++)
		grid[y][gridsz-1-x] = orig[y][x];
}

static void
render(void)
{
	int i=0, tx,ty, x0,y0, x,y;
	struct tile *tile;
	
	for (ty=0; ty<layoutsz; ty++)
	for (tx=0; tx<layoutsz; tx++) {
		tile = layout[i++];
		y0 = ty * (tilesz-2);
		x0 = tx * (tilesz-2);

		for (y=0; y<tilesz-2; y++)
		for (x=0; x<tilesz-2; x++)
			grid[y0+y][x0+x] =  tile->g[y+1][x+1];
	}
}

/* 1 if tile, in slot, fits its top and left neighbours */
static int
testslot(struct tile *tile, int slot)
{
	struct tile *top, *left;
	int x,y;

	if (slot > layoutsz) {
		top = layout[slot-layoutsz];
		for (x=0; x<tilesz; x++)
			if (top->g[tilesz-1][x] != tile->g[0][x])
				return 0;
	}

	if (slot % layoutsz != 0) {
		left = layout[slot-1];
		for (y=0; y<tilesz; y++)
			if (left->g[y][tilesz-1] != tile->g[y][0])
				return 0;
	}

	return 1;
}

/*
 * Try tos solve layout[] recursively from layout[slot] using any of
 * the tiles in layout[slot..]
 */
static int
dolayout(int slot)
{
	int cand, mut;
	struct tile *tmp;

	assert(slot >= 0 && slot < ntiles);

	for (cand=slot; cand<ntiles; cand++)
	for (mut=0; mut<8; mut++) {
		if (testslot(layout[cand], slot)) {
			if (slot == ntiles-1)
				return 1;
			tmp = layout[slot];

			/* provisially assign slot and recurse */
			layout[slot] = layout[cand];
			layout[cand] = tmp;
			if (dolayout(slot+1))
				return 1;

			/* no dice, restore slots */
			layout[cand] = layout[slot];
			layout[slot] = tmp;
		}

		if ((mut+1) % 4 == 0)
			flip(layout[cand]);
		rotate(layout[cand]);
	}

	return 0;
}

static int
hunt(void)
{
	int mut, gx,gy, mx,my, nhits=0;
	
	for (mut=0; mut<8; mut++) {
		for (gy=0; gy<gridsz-LEN(monster); gy++)
		for (gx=0; gx<gridsz-LEN(monster[0])+1; gx++) {
			for (my=0; my<LEN(monster); my++)
			for (mx=0; mx<LEN(monster[0])-1; mx++)
				if (monster[my][mx] == '#' &&
				    !grid[gy+my][gx+mx])
					goto nonessie;

			nhits++;

			for (my=0; my<LEN(monster); my++)
			for (mx=0; mx<LEN(monster[0])-1; mx++)
				if (monster[my][mx] == '#')
					grid[gy+my][gx+mx] = 0;
		nonessie: ;
		}

		if (nhits)
			return nhits;
		
		if ((mut+1) % 4 == 0)
			flipgrid();
		rotategrid();
	}

	return 0;
}

int
main(int argc, char **argv)
{
	FILE *f;
	int64_t p1;
	int p2=0, x,y;

	f = argc > 1 ? fopen(argv[1], "r") : stdin;
	assert(f);

	parse(f);

	if (!dolayout(0)) {
		printf("no p1 solution\n");
		return 0;
	}

	p1 = (int64_t)
	    layout[0]->id *
	    layout[layoutsz-1]->id *
	    layout[ntiles-layoutsz]->id *
	    layout[ntiles-1]->id;

	render();

	if (!hunt()) {
		printf("no nessie\n");
		return 0;
	}

	for (y=0; y<gridsz; y++)
	for (x=0; x<gridsz; x++)
		p2 += grid[y][x];

	printf("%" PRId64 " %d\n", p1, p2);
	//getchar();
	return 0;
}
