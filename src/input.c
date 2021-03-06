#include <SDL/SDL.h>
#include <stdbool.h>

#include "input.h"
#include "gltris.h"
#include "timer.h"
#include "block.h"
#include "game.h"

void handle_input(void){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch(g_game.gamestate){
    case STATE_GAME:
      switch(event.type){				
      case SDL_QUIT:
	g_game.running = false;
	break;
				
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_ESCAPE:
	  pause_timer(g_fall_timer);
	  g_game.gamestate = STATE_PAUSE;
	  break;
	}
	if(g_player.type != NULL_PIECE){
	  switch(event.key.keysym.sym){
	  case SDLK_RIGHT:
	    g_player.move = RIGHT;
	    break;
						
	  case SDLK_LEFT:
	    g_player.move = LEFT;
	    break;
						
	  case SDLK_UP:
	    g_player.snap = true;
	    break;
					
	  case SDLK_d:
	    g_player.rotate = RIGHT;
	    break;

	  case SDLK_a:
	    g_player.rotate = LEFT;
	    break;

	  default:
	    g_player.rotate = NONE;
	    g_player.snap = false;
	    g_player.move = NONE;
	    break;
	  }
	}
      }
      break;
    case STATE_MENU:
      switch(event.type){
      case SDL_QUIT:
	g_game.running = false;
	break;
      }
      break;
			
    case STATE_SPLASH:
      switch(event.type){
      case SDL_QUIT:
	g_game.running = false;
	break;
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_RETURN:
	  g_game.gamestate = STATE_GAME;
	  break;
	}
      }
      break;

    case STATE_PAUSE:
      switch(event.type){
      case SDL_QUIT:
	g_game.running = false;
	break;
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_ESCAPE:
	  resume_timer(g_fall_timer);
	  g_game.gamestate = STATE_GAME;
	  break;
	}
      }
      break;

    case STATE_GAMEOVER:
      switch(event.type){
      case SDL_QUIT:
	g_game.running = false;
	break;
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_RETURN:
	  g_game.gamestate = STATE_GAME;
	  do_reset();
	  break;
	}
      }
      break;
    }
  }
}
