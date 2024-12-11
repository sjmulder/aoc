struct vis {
	uint8_t *frame;
	size_t w, h;

	pid_t ffmpeg_pid;
	FILE *ffmpeg_file;
};

struct vis_grid {
	uint8_t *data;
	uint8_t colors[256][3];
	uint8_t transparent[256];
	size_t x,y, w,h;
	size_t cell_sz, border;
};

void vis_begin(
    struct vis *,
    char *filename,
    size_t fps, size_t w, size_t h);

void vis_fill(
    struct vis *,
    size_t x0, size_t y0, size_t w, size_t h,
    uint8_t r, uint8_t g, uint8_t b);

void vis_draw_grid(struct vis *, struct vis_grid *);
void vis_emit(struct vis *, size_t duration);
void vis_end(struct vis *);

void lerp_rainbow(float f, uint8_t *r, uint8_t *g, uint8_t *b);
