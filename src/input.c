#include <SDL/SDL.h>
#include <stdbool.h>


#include "input.h"
#include "gltris.h"
#include "timer.h"
#include "block.h"

void handle_input(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			g_running = false;
			break;

		case SDL_KEYDOWN:
			if(g_player.type != NULL_PIECE){
				switch(event.key.keysym.sym)
				{
				case SDLK_RIGHT:
					if(check_collisions(g_player.x+1, g_player.y, g_player.rotation) == NO_COLLISION)
						g_player.x += 1;
					break;
					
				case SDLK_LEFT:
					if(check_collisions(g_player.x-1, g_player.y, g_player.rotation) == NO_COLLISION)
						g_player.x -= 1;
					break;
					
				case SDLK_SPACE:
					g_player.rotation = g_player.rotation != 3 ? g_player.rotation + 1 : 0;
					do_wallkicks();
					break;
				}
			}
		}
		
	}
}
