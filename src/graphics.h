#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WWIDTH 480
#define WHEIGHT 480
#define NUMFPS 30

extern SDL_Surface* g_display;

int graphics_init(void);
void graphics_begin_frame(void);
void graphics_end_frame(void);
void graphics_render_block(int, int, const uint8_t*);
void graphics_render_blockgrid(void);
void do_rotation(float, float, float *, float *, float);

typedef uint8_t rgb_t[3];
typedef uint8_t rgba_t[4];

extern const uint8_t g_piece_colors[];

#endif
