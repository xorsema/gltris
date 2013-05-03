#ifndef GLTRIS_H
#define GLTRIS_H


#define GRIDSZX 10
#define GRIDSZY 22
#define BLOCKSIZE 24

enum
{
	STATE_GAME,
	STATE_MENU,
	STATE_SPLASH
};

typedef struct game_info
{
	int gamestate;
	bool running;/* Is the game running? */
	unsigned int score;
	unsigned int level;
} game_info_t;

extern uint8_t g_blockgrid[GRIDSZX][GRIDSZY];
extern game_info_t g_game;

#endif
