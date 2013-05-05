#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "gltris.h"
#include "graphics.h"
#include "input.h"
#include "block.h"
#include "timer.h"
#include "text.h"

uint8_t g_blockgrid[GRIDSZX][GRIDSZY];

game_info_t g_game;

int gltris_init(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;
	if(graphics_init() != 0)
		return 1;
	
	if(text_init() != 0)
		return 1;

	memset(g_blockgrid, 0, sizeof(g_blockgrid));
	srand(time(NULL));

	regenerate_bag();
//	spawn_piece(get_next_piece());
	
	return 0;
}

void gltris_loop()
{
	g_game.running = true;
	g_game.gamestate = STATE_SPLASH;

	g_second_timer = add_timer(1000, NULL);

	/* Main game loop */
	while(g_game.running)
	{
                /* timer.c: Update all the timers and call the appropriate callbacks */
		do_timers();

		/* input.c: handles keyboard etc */
		handle_input();
		
                /* graphics.c: sets up proper rendering context for this frame */
		graphics_begin_frame();

		/* Do different things depending on the gamestate */
		switch(g_game.gamestate)
		{
		case STATE_GAME:
			/* block.c: handles block behaviour */
			handle_blocks();
			
			/* graphics.c: renders the player's piece */
			graphics_render_player();
			
			/* graphics.c: renders the blocks */
			graphics_render_blockgrid();
			break;
			
		case STATE_SPLASH:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2i(100, 100);
			text_print("I am a test");
			break;

		default:
			break;
		}

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
