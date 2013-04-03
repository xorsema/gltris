#include <SDL/SDL.h>
#include <stdbool.h>


#include "input.h"
#include "gltris.h"
#include "timer.h"

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
		}
	}
}
