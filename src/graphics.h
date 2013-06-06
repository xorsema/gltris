#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WWIDTH 480
#define WHEIGHT 480
#define NUMFPS 30

#define GHOSTTRANS 70

extern SDL_Surface* g_display;

int graphics_init(void);
void graphics_begin_frame(void);
void graphics_end_frame(void);
void graphics_render_block(float, float, const uint8_t*, bool);
void graphics_render_grid_block(int, int, const uint8_t*);
void graphics_render_blockgrid(void);
void graphics_render_player(void);
void graphics_render_piece_preview(void);
void do_rotation(float, float, float *, float *, float);
int surface_to_texture(SDL_Surface *, GLuint *, bool, GLenum);
void render_textured_quad(GLuint, float, float, float, float);
void graphics_render_ghost_piece(void);

typedef uint8_t rgb_t[3];
typedef uint8_t rgba_t[4];

extern const uint8_t g_piece_colors[];

#endif
