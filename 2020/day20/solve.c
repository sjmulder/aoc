#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEN(a) ((int)(sizeof(a)/sizeof(*(a))))

/*
 * Dictionary:
 *  tile   - single block
 *  layout - arrangement of tiles (references)
 *  slot   - position in a layout
 *  grid   - rendered layout
 */

struct tile {
	int id;
	char g[10][10];
};

static struct tile tiles[144];
static struct tile *layout[144];
static int ntiles;
static int tilesz;
static int layoutsz;

static void
parse(void)
{
	int id, c, x,y;
	struct tile *tile;

	while (1) {
		if (scanf("Tile %d:\n", &id) != 1)
			break;

		assert(ntiles < LEN(tiles));

		tile = &tiles[ntiles];
		layout[ntiles] = tile;
		ntiles++;
		x = y = 0;

		while (layoutsz*layoutsz < ntiles)
			layoutsz++;

		while ((c = getchar()) != EOF) {
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
flip(struct tile *tile)
{
	struct tile orig;
	int x,y;

	memcpy(&orig, tile, sizeof(orig));

	for (y=0; y<tilesz; y++)
	for (x=0; x<tilesz; x++)
		tile->g[y][tilesz-1-x] = orig.g[y][x];
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
solve(int slot)
{
	int cand, mut;
	struct tile *tmp;

	assert(slot >= 0 && slot < ntiles);

#if 0
	printf("solve:");
	for (i=0; i<slot; i++)
		printf(" %d", layout[i]->id);
	putchar('\n');
#endif

	for (cand=slot; cand<ntiles; cand++)
	for (mut=0; mut<8; mut++) {
		//printf("  cand %d, mut %d\n", cand, mut);

		if (testslot(layout[cand], slot)) {
			if (slot == ntiles-1)
				return 1;
			tmp = layout[slot];

			/* provisially assign slot and recurse */
			layout[slot] = layout[cand];
			layout[cand] = tmp;
			if (solve(slot+1))
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

static void
dump_tile(struct tile *tile)	/* without title */
{
	int x,y;

	for (y=0; y < tilesz; y++) {
		for (x=0; x < tilesz; x++)
			putchar(tile->g[y][x] ? '#' : '.');
		putchar('\n');
	}
}

static void
dump_tiles(void)
{
	int i;
	struct tile *tile;

	for (i=0; i < ntiles; i++) {
		tile = &tiles[i];
		printf("Tile %d:\n", tile->id);
		dump_tile(tile);
		putchar('\n');
	}
}

static void
dump_layout(int n)
{
	int i;

	for (i=0; i<n; i++) {
		assert(layout[i]);
		if (i && i % layoutsz == 0)
			putchar('\n');
		printf("%8d", layout[i]->id);
	}

	putchar('\n');
}

static void
dump_stats(void)
{
	printf("%d %dx%d tiles = %dx%d layout\n", ntiles, tilesz,
	    tilesz, layoutsz, layoutsz);
}

static void
dump_mutations(struct tile *tile)
{
	int i;

	for (i=0; i<4; i++) {
		printf("Rotation %d:\n", i);
		dump_tile(tile);
		putchar('\n');
		rotate(tile);
	}

	for (i=0; i<2; i++) {
		printf("Flip %d:\n", i);
		dump_tile(tile);
		putchar('\n');
		flip(tile);
	}
}

int
main()
{
	parse();

	//dump_tiles();
	//dump_layout(ntiles);
	//dump_mutations(&tiles[0]);
	//dump_stats();

	if (solve(0))
		printf("%lld\n", (long long)
		    layout[0]->id *
		    layout[layoutsz-1]->id *
		    layout[ntiles-layoutsz]->id *
		    layout[ntiles-1]->id);
	else
		printf("not solved\n");

	//dump_layout(ntiles);
}
