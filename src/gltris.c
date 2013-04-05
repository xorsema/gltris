#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "gltris.h"
#include "graphics.h"
#include "input.h"
#include "block.h"
#include "timer.h"

/* Is the game running? */
bool g_running;

/* 2d array representing blocks in a grid, less than 0 == set block, greater than 0 == moving (player interaction), 0 == no block */
int8_t g_blockgrid[GRIDSZX][GRIDSZY];

int gltris_init(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;
	if(graphics_init() != 0)
		return 1;
	
	memset(g_blockgrid, 0, sizeof(g_blockgrid));

	spawn_piece(I_PIECE);
	
	return 0;
}

void gltris_loop()
{
	g_running = true;

	g_second_timer = add_timer(1000, NULL);

	/* Main game loop */
	while(g_running)
	{
                /* timer.c: Update all the timers and call the appropriate callbacks */
		do_timers();

		/* block.c: tests for collisions */
		do_collisions();

		/* input.c: handles keyboard etc */
		handle_input();
		
		/* block.c: handles block behaviour */
		handle_blocks();

                /* graphics.c: sets up proper rendering context for this frame */
		graphics_begin_frame();

                /* graphics.c: renders the blocks */
		graphics_render_blockgrid();

		/* graphics.c: swaps buffers, etc */
		graphics_end_frame();
	}
}

int main(int argc, char **argv)
{
	if(gltris_init() != 0)
		return 1;
	gltris_loop();
	
	SDL_Quit();

	return 0;
}
