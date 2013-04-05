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
				if(!(g_player_collisions & COLLISION_RIGHT) && (g_player.x < GRIDSZX))
					g_player.x += 1;
				break;

			case SDLK_LEFT:
				if(!(g_player_collisions & COLLISION_LEFT) && (g_player.x > 0))
					g_player.x -= 1;
				break;
			}
		}
	}
}
