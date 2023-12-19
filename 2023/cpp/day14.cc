#include <cassert>
#include <cstdio>
#include <cstring>

#define LEN(x) (sizeof(x) / sizeof(*(x)))
#define DISCARD(x) ((void)!(x))

static char g[101][102];
static int w, h;

static int hash_grid(void) {
  int acc = 0;

  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      acc = acc * 31 + g[y][x];

  return acc;
}

static int score_grid(void) {
  int acc = 0;

  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      if (g[y][x] == 'O')
        acc += h - y;

  return acc;
}

static void shift_north(void) {
  /*
   * Walk two cursors i and j through each column x. The i cursor
   * looks for the first . where an O can go. The j cursor looks
   * ahead for O's. When j finds a # we start again beyond it.
   */
  for (int x = 0; x < w; x++)
    for (int i = 0, j = 1; i < h && j < h;)
      if (j <= i)
        j = i + 1;
      else if (g[j][x] == '#')
        i = j + 1;
      else if (g[j][x] != 'O')
        j++;
      else if (g[i][x] != '.')
        i++;
      else {
        g[i++][x] = 'O';
        g[j++][x] = '.';
      }
}

static void shift_all(void) {
  shift_north();

  for (int y = 0; y < w; y++)
    for (int i = 0, j = 1; i < w && j < w;)
      if (j <= i)
        j = i + 1;
      else if (g[y][j] == '#')
        i = j + 1;
      else if (g[y][j] != 'O')
        j++;
      else if (g[y][i] != '.')
        i++;
      else {
        g[y][i++] = 'O';
        g[y][j++] = '.';
      }

  for (int x = 0; x < w; x++)
    for (int i = h - 1, j = h - 2; i >= 0 && j >= 0;)
      if (j >= i)
        j = i - 1;
      else if (g[j][x] == '#')
        i = j - 1;
      else if (g[j][x] != 'O')
        j--;
      else if (g[i][x] != '.')
        i--;
      else {
        g[i--][x] = 'O';
        g[j--][x] = '.';
      }

  for (int y = 0; y < w; y++)
    for (int i = w - 1, j = w - 2; i >= 0 && j >= 0;)
      if (j >= i)
        j = i - 1;
      else if (g[y][j] == '#')
        i = j - 1;
      else if (g[y][j] != 'O')
        j--;
      else if (g[y][i] != '.')
        i--;
      else {
        g[y][i--] = 'O';
        g[y][j--] = '.';
      }
}

int main(int argc, char **argv) {
  static int hist[256]; /* hashes */

  if (argc > 1)
    DISCARD(freopen(argv[1], "r", stdin));

  for (h = 0; fgets(g[h], LEN(*g), stdin); h++)
    assert(h + 1 < (int)LEN(g));

  assert(h > 0);
  w = strlen(g[0]) - 1;
  assert(w > 0);

  shift_north();
  int p1 = score_grid();

  int nhist = 0, period = 0;
  for (int nleft = 1 * 1000 * 1000 * 1000; nleft; nleft--) {
    shift_all();

    if (!period) {
      assert(nhist < (int)LEN(hist));
      hist[nhist++] = hash_grid();

      for (int i = 0; i < nhist - 1; i++)
        if (hist[i] == hist[nhist - 1]) {
          period = nhist - 1 - i;
          nleft = nleft % period;
          break;
        }
    }
  }

  int p2 = score_grid();

  printf("14: %d %d\n", p1, p2);
  return 0;
}
