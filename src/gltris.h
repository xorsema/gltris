#ifndef GLTRIS_H
#define GLTRIS_H

#define TITLETXT "GLtris"
#define SPLASHTXT "Press ENTER to start!"
#define PAUSETXT "Press ESCAPE again to return"
#define SCORETXT "Your score was %i"
#define GAMEOVERTXT "Press ENTER for a new game"

enum
{
	STATE_GAME,
	STATE_MENU,
	STATE_SPLASH,
	STATE_PAUSE,
	STATE_GAMEOVER
};

#endif
