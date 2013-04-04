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
			switch(event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if(!(g_player_collisions & COLLISION_RIGHT))
//					offset_player_right(1);
					g_player.x += 1;
				break;

			case SDLK_LEFT:
				if(!(g_player_collisions & COLLISION_LEFT))
//					offset_player_left(1);
					g_player.x -= 1;
				break;
			}
		}
	}
}
