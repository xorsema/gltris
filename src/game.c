#include <stdint.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "gltris.h"
#include "block.h"
#include "timer.h"
#include "game.h"

player_info_t g_player;
grabbag_t g_grabbag;

uint8_t g_blockgrid[GRIDSZX][GRIDSZY];
game_info_t g_game;

/* returns time in milliseconds that is the correct delay for each level */
uint32_t delay_for_level(uint32_t level)
{
	double ms;

	ms = 1000*(pow(TIMERDECRATE, level));
	return (ms < 1) ? 1 : (uint32_t)ms;
}

void regenerate_bag(void)
{
	int i, max, min;

	max = Z_PIECE;
	min = I_PIECE;

	for(i = 0; i < 7; i++)
	{
		g_grabbag.pieces[i] = rand() % (max-min+1)+min;
	}	

	g_grabbag.index = 0;
}

int get_next_piece(void)
{
	int ret;

	if(g_grabbag.index > 6)
		regenerate_bag();
	ret = g_grabbag.pieces[g_grabbag.index];
	g_grabbag.index += 1;
	return ret;
}

void handle_scoring(uint32_t cleared)
{
	g_game.rows_cleared += cleared;
	g_game.level = g_game.rows_cleared / ROWSPERLEVEL;
}
