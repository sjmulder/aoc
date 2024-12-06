#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include "vis.h"

static int max(int a, int b) { return a>b ? a : b; }

void
vis_begin(
    struct vis *vis,
    char *filename,
    size_t fps, size_t w, size_t h)
{
	int fds[2], i;
	char video_size[64], fps_str[32];

	char *argv[] = {
		"ffmpeg",
		"-loglevel", "warning",
		"-stats",
		"-f", "rawvideo",
		"-pixel_format", "rgb24",
		"-video_size", video_size,
		"-framerate", fps_str,
		"-i", "-",
		"-y",	/* overwrite existing file */
		"-pix_fmt", "yuv420p",
		filename,
		NULL
	};

	assert(vis);
	assert(filename);
	assert(w);
	assert(h);
	assert(fps);

	vis->w = w;
	vis->h = h;
	vis->frame = malloc(w*h*3);

	snprintf(video_size, sizeof(video_size), "%zux%zu", w, h);
	snprintf(fps_str, sizeof(fps_str), "%zu", fps);

	putchar('+');
	for (i=0; argv[i]; i++)
		printf(" %s", argv[i]);
	putchar('\n');

	if (pipe(fds) == -1)
		err(1, "pipe");

	switch ((vis->ffmpeg_pid = fork())) {
	case -1:
		err(1, "fork");
	case 0:
		dup2(fds[0], 0);
		close(fds[1]);
		execvp("ffmpeg", argv);
		err(1, "ffmpeg");
	}

	close(fds[0]);

	if (!(vis->ffmpeg_file = fdopen(fds[1], "w")))
		err(1, "fdopen");
}

void
vis_fill(
    struct vis *vis,
    size_t x0, size_t y0, size_t w, size_t h,
    uint8_t r, uint8_t g, uint8_t b)
{
	size_t x,y, off;

	assert(vis);
	assert(vis->frame);

	if (x0 >= vis->w) return;
	if (y0 >= vis->h) return;

	if (x0+w > vis->w) w = vis->w - x0;
	if (y0+h > vis->h) h = vis->h - y0;

	for (y=y0; y < y0+h; y++)
	for (x=x0; x < x0+w; x++) {
		off = y * vis->w *3 + x*3;
		vis->frame[off+0] = r;
		vis->frame[off+1] = g;
		vis->frame[off+2] = b;
	}
}

void
vis_draw_grid(struct vis *vis, struct vis_grid *grid)
{
	size_t x,y;
	uint8_t val;

	assert(vis);
	assert(grid);
	assert(grid->data);
	assert(grid->cell_sz);
	assert(grid->border*2 < grid->cell_sz);
	assert(grid->border*2 < grid->cell_sz);

	for (y=0; y < grid->h; y++)
	for (x=0; x < grid->w; x++) {
		val = grid->data[y * grid->w + x];

		if (grid->transparent[val])
			continue;

		vis_fill(vis,
		    grid->x + x * grid->cell_sz + grid->border,
		    grid->y + y * grid->cell_sz + grid->border,
		    grid->cell_sz - grid->border*2,
		    grid->cell_sz - grid->border*2,
		    grid->colors[val][0],
		    grid->colors[val][1],
		    grid->colors[val][2]);
	}
}

void
vis_emit(struct vis *vis, size_t duration)
{
	size_t i;

	assert(vis);
	assert(vis->frame);
	assert(vis->ffmpeg_file);

	for (i=0; i < duration; i++)
		fwrite(vis->frame, vis->w * vis->h *3, 1,
		    vis->ffmpeg_file);
}

void
vis_end(struct vis *vis)
{
	int status;

	assert(vis);
	assert(vis->ffmpeg_pid);
	assert(vis->ffmpeg_file);

	fclose(vis->ffmpeg_file);

	if (waitpid(vis->ffmpeg_pid, &status, 0) == -1)
		err(1, "waitpid");
	if (status)
		errx(1, "ffmpeg exited with status %d", status);

	memset(vis, 0, sizeof(*vis));
}

void
lerp_rainbow(float f, uint8_t *r, uint8_t *g, uint8_t *b)
{
	int v = (int)(f * 255*3);

	*r = (uint8_t)(max(0, 255-abs(v-255*3)) + max(0, 255-v));
	*g = (uint8_t) max(0, 255-abs(v-255*1));
	*b = (uint8_t) max(0, 255-abs(v-255*2));
}
