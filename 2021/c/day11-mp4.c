#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

#define SZ		10
#define FLASHED		('9'+2)

#define TILE_SZ		100
#define BORDER_SZ	1
#define FRAME_RPT	3
#define VIDEO_SZ	(TILE_SZ*SZ)

static char grid[SZ][SZ+1];
static FILE *ffmpeg;

static const uint8_t palette[10][3] = {
	{ 128, 255, 255 },
	{ 128, 128, 255 },
	{   0, 255, 255 },
	{ 255, 128, 255 },
	{ 255, 128, 128 },
	{ 255,   0, 255 },
	{ 255, 255, 128 },
	{ 128, 255, 128 },
	{ 255, 255,   0 },
	{ 128, 128, 128 }
};
/*
static const uint8_t palette[10][3] = {
	{   0,  25,  25 },
	{   0,  50,  50 },
	{   0,  75,  75 },
	{ 100,   0, 100 },
	{ 125,   0, 125 },
	{ 150,   0, 150 },
	{ 175, 175,   0 },
	{ 200, 200,   0 },
	{ 225, 225,   0 },
	{ 255, 255, 255 }
};
*/
static void
start_ffmpeg()
{
	char szarg[20], **p; 
	char *argv[] = {
	    "ffmpeg4", "-f", "rawvideo", "-vcodec", "rawvideo",
	    "-s", szarg, "-pix_fmt", "rgb24", "-r", "60", "-i", "-",
	    "-an", "-vcodec", "mpeg4", "day11.mp4", NULL };
	int fds[2];

	snprintf(szarg, sizeof(szarg), "%dx%d", VIDEO_SZ, VIDEO_SZ);

	for (p=argv; *p; p++)
		printf("%s ", *p);
	putchar('\n');

	if (pipe(fds) == -1)
		err(1, "pipe()");

	switch (fork()) {
	case -1:
		err(1, "fork()");
	case 0:
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		execvp("ffmpeg4", argv);
		err(1, "ffmpeg4");
	}

	close(fds[0]);
	if (!(ffmpeg = fdopen(fds[1], "w")))
		err(1, "fdopen");
}

static void
send_frame(int step)
{
	static uint8_t frame[VIDEO_SZ][VIDEO_SZ][3];
	int r,c, x,y, i;

	for (r=0; r<SZ; r++)
	for (c=0; c<SZ; c++)
	for (y=r*TILE_SZ+BORDER_SZ; y<(r+1)*TILE_SZ-BORDER_SZ; y++)
	for (x=c*TILE_SZ+BORDER_SZ; x<(c+1)*TILE_SZ-BORDER_SZ; x++)
		memcpy(frame[y][x], palette[(grid[r][c]-'0' +9 -step%10) %10], 3);

	for (i=0; i<FRAME_RPT; i++)
		if (fwrite(frame, sizeof(frame), 1, ffmpeg) != 1)
			err(1, "fwrite()");
}

static void
flash(int r, int c)
{
	int r2,c2;

	if (grid[r][c] == FLASHED)
		return;
	grid[r][c] = FLASHED;

	for (r2=r-1; r2<=r+1; r2++)
	for (c2=c-1; c2<=c+1; c2++) {
		if (r2<0 || r2>=SZ) continue;
		if (c2<0 || c2>=SZ) continue;
		if (r==r2 && c==c2) continue;

		if (grid[r2][c2] <= '9' && ++grid[r2][c2] > '9')
			flash(r2, c2);
	}
}

int
main()
{
	int i,r,c, p1=0, nflash=0;

	if (fread(grid, 1, sizeof(grid), stdin) == 1)
		assert(!"bad input");

	start_ffmpeg();
	send_frame(0);

	for (i=0; nflash != SZ*SZ; i++) {
		nflash=0;

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			grid[r][c]++;

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			if (grid[r][c] > '9')
				flash(r, c);

		for (r=0; r<SZ; r++)
		for (c=0; c<SZ; c++)
			if (grid[r][c] == FLASHED) {
				nflash++;
				grid[r][c] = '0';
			}

		send_frame(i+1);

		if (i<100)
			p1 += nflash;
	}

	printf("11: %d %d\n", p1, i);
	fclose(ffmpeg);
	wait(NULL);

	return 0;
}
