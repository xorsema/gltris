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
piece_pool_t g_pool;

uint8_t g_blockgrid[GRIDSZX][GRIDSZY];
game_info_t g_game;

/* returns time in milliseconds that is the correct delay for each level */
uint32_t delay_for_level(uint32_t level)
{
	double ms;

	ms = 1000*(pow(TIMERDECRATE, level));
	return (ms < 1) ? 1 : (uint32_t)ms;
}

void fill_pool(unsigned int *p)
{
	int i;

	for(i = 0; i < 7; i++)
	{
		p[i] = rand() % (Z_PIECE-I_PIECE+1)+I_PIECE;
	}
}

int get_piece(void)
{
	int ret;

	/* If we've reached the end (7 - 1) */
	if(g_pool.index == 6){
		g_pool.index = 0;/* Start over at the beginning */
		memcpy(g_pool.prim, g_pool.sec, 7*sizeof(unsigned int));/* Copy over the sec pool into the prim */
		fill_pool(g_pool.sec);/* Refill the sec pool */
	}

	ret = g_pool.prim[g_pool.index];

	g_pool.index++;

	return ret;
}

int peek_piece(void)
{
	/* If the index is 7, get_piece hasn't been called again, so we need to get the next piece from the sec pool */
	return (g_pool.index == 6) ? (g_pool.sec[0]) : (g_pool.prim[g_pool.index]);
}

void handle_scoring(uint32_t cleared)
{
	g_game.rows_cleared += cleared;
	g_game.level = g_game.rows_cleared / ROWSPERLEVEL;
}
