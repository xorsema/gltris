#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "graphics.h"
#include "block.h"
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

/* Must be called within a proper OpenGL context, renders a block in a grid of blocks (as in g_blockgrid) */
void graphics_render_block(int x, int y, const uint8_t* color)
{
	glPushMatrix(); /* Save the matrix so other stuff doesn't get messed up */
	
	glTranslatef((x * BLOCKSIZE), (y * BLOCKSIZE), 0.0f); /* Move to the proper position in the grid */
	glScalef(BLOCKSIZE, BLOCKSIZE, 0.0f); /* set size (in pixels because of gluOrtho2d) */
	glColor3ubv(color); /* Make sure we're drawing in the proper color */
	
	/* Render our block with GL_QUADS */
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(1, 0);
	glVertex2i(1, 1);
	glVertex2i(0, 1);
	glEnd();

	glPopMatrix(); /* Restore the matrix we saved previously */
}

/* Render the player piece, which is separate from other pieces already placed */
void graphics_render_player(void)
{
	int i, j;
	uint8_t b;
	for(i = 0; i < PBLOCKMAX; i++)
	{
		for(j = 0; j < PBLOCKMAX; j++)
		{
			b = get_player_block(i, j);
			if(b != 0){
				graphics_render_block(i+g_player.x, j+g_player.y, &g_piece_colors[b*3]);
			}
		}
	}

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
			if((b = g_blockgrid[i][j]) != 0){
				graphics_render_block(i, j, &g_piece_colors[abs(b)*3]); /* abs() because we don't want negatives to deref the color */
			}
		}
		
	}
}
