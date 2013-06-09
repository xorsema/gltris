#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "graphics.h"
#include "block.h"
#include "game.h"
#include "gltris.h"

SDL_Surface* g_display;

/* table of colors, dereferencing this table with the ID * 3 should give the proper color, 0 being for NULL value */
const uint8_t g_piece_colors[] = {
	0, 0, 0,       // NULL
	255, 0, 0,     // red(1) I
	255, 0, 255,   // magenta(2) J
	255, 255, 0,   // yellow(3) L
	0, 255, 255,   // cyan(4) O
	0, 0, 255,     // blue(5) S
	192, 192, 192, // light grey(6) T
	0, 255, 0      // green(7) Z
};

/* Sets up the display and OpenGL */
int graphics_init(void)
{
	/* Init the display surface (SDL) */
	if((g_display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
		return 1;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, WWIDTH, WHEIGHT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST); /* We don't need this for 2d */
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	return 0;
}

/* Set up the frame for drawing */
void graphics_begin_frame(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	
	gluOrtho2D(0, WWIDTH, 0, WHEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void graphics_end_frame(void)
{
	SDL_GL_SwapBuffers();
}

/* Render a colored quad to the screen, requires a proper OpenGL context */
void graphics_render_colored_quad(float x, float y, float w, float h, const uint8_t *color, uint8_t *alpha)
{
	glPushMatrix(); /* Save the matrix so other stuff doesn't get messed up */
	
	glTranslatef(x, y, 0.0f); /* Move to the proper position */
	glScalef(w, h, 0.0f); /* set size (in pixels because of gluOrtho2d) */

	/* Make sure we're drawing in the proper color, draw with an alpha channel and blend, if one is specified */
	if(alpha != NULL)
		glColor4ub(color[0], color[1], color[2], *alpha);
	else
		glColor3ubv(color);

	glEnable(GL_BLEND);
	
	/* Render our quad with GL_QUADS */
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(1, 0);
	glVertex2i(1, 1);
	glVertex2i(0, 1);
	glEnd();

	glDisable(GL_BLEND);

	glPopMatrix(); /* Restore the matrix we saved previously */
}

/* Render a block in an arbitrary location on the screen, must be called within a proper OpenGL context */
void graphics_render_block(float x, float y, const uint8_t* color, bool ghost)
{
	uint8_t alpha = GHOSTTRANS;

	if(ghost)
		graphics_render_colored_quad(x, y, BLOCKSIZE, BLOCKSIZE, color, &alpha);
	else
		graphics_render_colored_quad(x, y, BLOCKSIZE, BLOCKSIZE, color, NULL);
}

/* Must be called within a proper OpenGL context, renders a block in a grid of blocks (as in g_blockgrid) */
void graphics_render_grid_block(int x, int y, const uint8_t* color)
{
	graphics_render_block(x * BLOCKSIZE, y * BLOCKSIZE, color, false); /* x/y position is the size of the block multiplied by the position in the grid */
}

/* Render a piece in an arbitrary location */
void graphics_render_piece(float x, float y, unsigned int type, unsigned int inrot, bool ghost)
{
        int		i, j;
	int		b;
	int		max;
	piece_ptr_t	piece;
	
	max   = get_piece_size(type);
	piece = block_pointer_from_type(type);

	for(j = 0; j < max; j++)
	{
		for(i = 0; i < max; i++)
		{
			b = get_block(i, j, inrot, type, piece);
			if(b != 0)
				graphics_render_block((i*BLOCKSIZE)+x, (j*BLOCKSIZE)+y, &g_piece_colors[b*3], ghost);
		}
	}
}

/* Render the player piece, which is separate from other pieces already placed */
void graphics_render_player(void)
{
	if(g_player.type == NULL_PIECE)
		return;

	graphics_render_piece(g_player.x*BLOCKSIZE, g_player.y*BLOCKSIZE, g_player.type, g_player.rotation, false);
}

void graphics_render_piece_preview(void)
{
	int p;
	
	p = peek_piece();
	graphics_render_piece(WWIDTH - 4*BLOCKSIZE, WHEIGHT - 200, p, 1, false);
}

void graphics_render_ghost_piece(void)
{
	int x, y;

	if(g_player.type == NULL_PIECE)
		return;

	get_ghost_info(&x, &y);
	graphics_render_piece(x * BLOCKSIZE, y * BLOCKSIZE, g_player.type, g_player.rotation, true);
}

/* Render our grid of blocks, using the piece colors array */
void graphics_render_blockgrid(void)
{
	int i, j;
	int8_t b;
	
	for(i = 0; i < GRIDSZX; i++)
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			if((b = g_blockgrid[i][j]) != 0)
				graphics_render_grid_block(i, j, &g_piece_colors[b*3]); 
		}
		
	}
}

void render_textured_quad(GLuint texname, float x, float y, float w, float h)
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texname);
	
	glTranslatef(x, y, 0.0);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, h); 
	
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0, 0); 
		
	glTexCoord2f(1.0, 1.0);
	glVertex2f(w, 0.0); 
	
	glTexCoord2f(1.0, 0.0);
	glVertex2f(w, h); 

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

/* Convert an SDL_Surface to an OpenGL texture, return 0 on success */
int surface_to_texture(SDL_Surface *s, GLuint *tn, bool free, GLenum fmt)
{
	GLenum	format;
	GLuint	result;

	/* If the format isn't specified, attempt to guess it */
	if(fmt != 0)
		format = fmt;
	else if(s->format->BytesPerPixel == 3)
		format = GL_RGB;
	else if(s->format->BytesPerPixel == 4)
		format = GL_RGBA;
	else 
		return 1;
	

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, s->format->BytesPerPixel, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);

	*tn = result;

	if(free)
		SDL_FreeSurface(s);

	return 0;
}
