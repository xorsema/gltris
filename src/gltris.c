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
#include "game.h"

int gltris_init(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;
	if(graphics_init() != 0)
		return 1;
	
	if(text_init() != 0)
		return 1;

	memset(g_blockgrid, 0, sizeof(g_blockgrid));
	memset(&g_game, 0, sizeof(game_info_t));
	srand(time(NULL));

	fill_pool(g_pool.prim);
	fill_pool(g_pool.sec);
	
	return 0;
}

void gltris_loop()
{
	uint32_t w, h;

	g_game.running = true;
	g_game.gamestate = STATE_SPLASH;

	g_fall_timer = add_timer(STARTINGTIME, NULL);

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
			
			graphics_render_ghost_piece();

			/* graphics.c: renders the blocks */
			graphics_render_blockgrid();

			/* text.c: print info such as score, level */
			print_game_info();

			/* graphics.c: render a preview of the next piece */
			graphics_render_piece_preview();

			break;
			
		case STATE_SPLASH:
			get_text_size(TITLETXT, 16, &w, &h);
			text_print((WWIDTH-w)/2, (WHEIGHT-h)/2, 16, TITLETXT);
			get_text_size(SPLASHTXT, 16, &w, &h);
			text_print((WWIDTH-w)/2, (WHEIGHT-h)/2 - 50, 16, SPLASHTXT);
			break;

		case STATE_PAUSE:
			glColor3f(1.0f, 1.0f, 1.0f);
			get_text_size(PAUSETXT, 16, &w, &h);
			text_print((WWIDTH-w)/2, (WHEIGHT-h)/2, 16, PAUSETXT);

		case STATE_GAMEOVER:
			print_gameover_text();
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
