#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>

#define LEN(a)	(sizeof(a)/sizeof(*(a)))

#define SZY	48
#define SZX	144

#define VIS_CELL_SZ	20
#define VIS_BORDER_SZ	2
#define VIS_FRAME_TIME	3

static char map[SZY][SZX];
static int dist[SZY][SZX];
static int w,h, sx,sy, ex,ey;

static FILE* ffmpeg_file;
static pid_t ffmpeg_pid;
static int img_w, img_h;

static int max(int a, int b) { return a>b ? a : b; }

static int
step(int dx, int dy)
{
	int nchange=0, x,y;

	for (y = max(0, -dy); y < h && y+dy < h; y++)
	for (x = max(0, -dx); x < w && x+dx < w; x++)
		if (map[y][x] >= map[y+dy][x+dx]-1 &&
		    dist[y][x]-1 > dist[y+dy][x+dx]) {
			dist[y][x] = dist[y+dy][x+dx]+1;
			nchange++;
		}

	return nchange;
}

static FILE *
open_ffmpeg(void)
{
	int fds[2], i;
	char video_size[64];
	FILE *f;

	char *argv[] = {
		"ffmpeg",
		"-f", "rawvideo",
		"-pixel_format", "rgb24",
		"-video_size", video_size,
		"-framerate", "30",
		"-i", "-",
		"day12.mp4",
		NULL
	};

	snprintf(video_size, sizeof(video_size), "%dx%d", img_w, img_h);

	printf("+");
	for (i=0; i < (int)LEN(argv); i++)
		printf(" %s", argv[i]);
	printf("\n");

	if (pipe(fds) == -1)
		err(1, "pipe");

	switch ((ffmpeg_pid = fork())) {
	case -1:
		err(1, "fork");
	case 0:
		dup2(fds[0], 0);
		close(fds[1]);
		execvp("ffmpeg", argv);
		err(1, "exec");
	}

	close(fds[0]);

	if (!(f = fdopen(fds[1], "w")))
		err(1, "fdopen");

	return f;
}

static void
fill(uint8_t *img,
    int x0, int y0, int w, int h,
    uint8_t r, uint8_t g, uint8_t b)
{
	int x,y;

	assert(x0 >= 0); assert(x0+w <= img_w);
	assert(y0 >= 0); assert(y0+h <= img_h);

	for (y=y0; y<y0+h; y++)
	for (x=x0; x<x0+w; x++) {
		img[y*img_w*3 + x*3 +0] = r;
		img[y*img_w*3 + x*3 +1] = g;
		img[y*img_w*3 + x*3 +2] = b;
	}
}

static void
emit_frame(void)
{
	static uint8_t img[
	    (SZX*VIS_CELL_SZ + (SZX+1)*VIS_BORDER_SZ) *
	    (SZY*VIS_CELL_SZ + (SZY+1)*VIS_BORDER_SZ) *3];
	int x,y, i;
	uint8_t r,g,b;

	assert((size_t)img_w*img_h*3 <= sizeof(img));

	fill(img, 0,0, img_w,img_h, 0,0,0);

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		r=g=b=0;

		if ((y==sy && x==sx) || (y==ey && x==ex))
			b = 255;
		else if (dist[y][x] == INT_MAX)
			g = (uint8_t)(10 + (map[y][x]-'a') *9);
		else
			r = (uint8_t)(10 + 245 - dist[y][x]/2);

		fill(img,
		    x * (VIS_CELL_SZ + VIS_BORDER_SZ),
		    y * (VIS_CELL_SZ + VIS_BORDER_SZ),
		    VIS_CELL_SZ,
		    VIS_CELL_SZ,
		    r, g, b);
	}

	for (i=0; i < VIS_FRAME_TIME; i++)
		fwrite(img, img_w*img_h*3, 1, ffmpeg_file);
}

int
main()
{
	char *lf;
	int x,y, nchange, status, i;

	while (fgets(map[h], SZX, stdin)) {
		if ((lf = strchr(map[h], '\n')))
			*lf = '\0';
		h++;
		assert(h < SZY);
	}

	assert(h);
	w = strlen(map[0]);

	img_w = w*VIS_CELL_SZ + (w+1)*VIS_BORDER_SZ;
	img_h = h*VIS_CELL_SZ + (h+1)*VIS_BORDER_SZ;

	for (y=0; y<h; y++)
	for (x=0; x<w; x++) {
		dist[y][x] = INT_MAX;

		switch (map[y][x]) {
		case 'S': sx = x; sy = y; map[y][x] = 'a'; break;
		case 'E': ex = x; ey = y; map[y][x] = 'z'; break;
		}
	}

	dist[ey][ex] = 0;

	ffmpeg_file = open_ffmpeg();
	emit_frame();

	do {
		nchange  = step( 1,0);
		nchange += step(-1,0);
		nchange += step(0, 1);
		nchange += step(0,-1);
		emit_frame();
	} while (nchange);

	for (i=0; i<30; i += VIS_FRAME_TIME)
		emit_frame();

	fclose(ffmpeg_file);

	while (wait(&status) != -1)
		if (status)
			warnx("child exited with status %d\n", status);
	if (errno != ECHILD)
		err(1, "wait");

	return 0;
}
