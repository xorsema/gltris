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
					g_player.move = RIGHT;
					break;
					
				case SDLK_LEFT:
					g_player.move = LEFT;
					break;
		
				case SDLK_UP:
					g_player.snap = true;
					break;

				case SDLK_SPACE:
					g_player.rotate = RIGHT;
					break;
				}
			}
		}
		
	}
}
