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
	
	g_blockgrid[9][19] = 1;
	g_blockgrid[10][19] = 1;
	g_blockgrid[9][18] = 1;
	g_blockgrid[8][18] = 1;
	g_blockgrid[10][0] = -1;

	return 0;
}

void gltris_loop()
{
	uint32_t pticks, nticks;
	int framecount;
	uint32_t sec_ticks;

	g_running = true;
	sec_ticks = pticks = SDL_GetTicks();
	framecount = 0;

	g_second_timer = add_timer(1000, NULL);

	/* Main game loop */
	while(g_running)
	{
                /* timer.c: Update all the timers and call the appropriate callbacks */
		do_timers();

		if(framecount == NUMFPS){ /* If we've reached the frame count limit for this frame */
			framecount = 0; /* reset the counter */
			nticks = SDL_GetTicks() - pticks; /* then get the time elapsed since the last time the limit was reached... */
			if(nticks < 1000){ /* ...and if the time is less than a second (1000 ms)... */
				SDL_Delay(1000 - nticks); /* ...sleep the remainder */
			}
			pticks = SDL_GetTicks(); /* Store the current time so we can do this again */
		}

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
		
		framecount++;
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
