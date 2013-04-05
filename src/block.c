#include <stdint.h>
#include <stdbool.h>
#include <SDL/SDL.h>

#include "gltris.h"
#include "block.h"
#include "timer.h"

collision_t g_player_collisions;
player_info_t g_player;

const uint8_t I_piece[4][4][4] = {

	{
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }
	}
	
};

const uint8_t J_piece[4][3][3] = {
	{
		{ 2, 0, 0 },
		{ 2, 2, 2 },
		{ 0, 0, 0 }
	},
	{
		{ 0, 2, 2 },
		{ 0, 2, 0 },
		{ 0, 2, 0 }
	},
	{
		{ 0, 0, 0 },
		{ 2, 2, 2 },
		{ 0, 0, 2 }
	},
	{
		{ 0, 2, 0 },
		{ 0, 2, 0 },
		{ 2, 2, 0 }
	}
};

void write_player(void)
{
	int i, j, max;
	int8_t t;
	max = (g_player.type == 1 || g_player.type == 4) ? 4 : 3;/* If it's a O or I piece it will be 4x4 */
	for(i = 0; i < max; i++)
	{
		int x = i + g_player.x;
		for(j = 0; j < max; j++)
		{
			int y = j + g_player.y;
			t = (g_player.type == 1 || g_player.type == 4) ? (*g_player.piece.a)[g_player.rotation][j][i] : (*g_player.piece.b)[g_player.rotation][j][i];
			if((t != 0) && g_blockgrid[x][y] == 0){
				g_blockgrid[x][y] = t;
			}
		}
	}
}

void clear_player(void)
{
	int i, j;
	for(i = 0; i < GRIDSZX; i++)
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			if(g_blockgrid[i][j] > 0){
				g_blockgrid[i][j] = 0;
			}
		}
	}
}

/* Move a block by the specified offsets */
void offset_block(int x, int y, int xoff, int yoff)
{
	int nx, ny;
	int8_t *o, *n;

	nx = x + xoff;
	ny = y + yoff;
	
//	if(nx < 0 || ny < 0 || nx > GRIDSZX || ny > GRIDSZY)
//		return;

	o = &g_blockgrid[x][y];
	n = &g_blockgrid[nx][ny];
	
	*n = *o;
	*o = 0;
}

/* Sets the player pieces, making them negative so they are effectively "placed" */
void set_pieces(void)
{
	int i, j;
	for(i = 0; i < GRIDSZX; i++)
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			int8_t *b = &g_blockgrid[i][j];
			if(*b > 0){
				*b = -(*b);
			}
		}
	}
}

/* Check for collisions between the player piece and the wall/floor/other blocks, store the result in g_player_collisions */
void do_collisions(void)
{
	int i, j;
	collision_t r = NO_COLLISION;
	for(i = 0; i < GRIDSZX; i++)
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			if(g_blockgrid[i][j] > 0){
				if((j == 0)  || (j != 0) && g_blockgrid[i][j-1] < 0){
					r |= COLLISION_BELOW;
				}
				
				if((i == 0) || /* If we're going to hit the left wall */
				   ((i != 0) && (g_blockgrid[i-1][j] < 0))){ /* Or going to hit a block to the left */
					r |= COLLISION_LEFT;
				}

				if((i == (GRIDSZX-1)) || /* If we're going to hit the right wall */
				   ((i != (GRIDSZX-1)) && (g_blockgrid[i+1][j] < 0))){ /* Or going to hit a block to the right */
					r |= COLLISION_RIGHT;
				}
					 
			}
		}
	}

	g_player_collisions = r;		
}

void do_gravity(void)
{
	if(g_second_timer->elapsed == true){/* If this frame falls on a second mark and there haven't been any collisions between the piece and a block/the ground */
		if(!(g_player_collisions & COLLISION_BELOW)){/* If it's safe to move the piece down */
			g_player.y -= 1;
		} else {
			set_pieces();/* "set" the piece's blocks (make them negative) so they aren't counted as a piece anymore */
			spawn_piece(2);
		}
	}
}

void handle_blocks(void)
{
	do_gravity();
	clear_player();
	write_player();
}

void spawn_piece(unsigned int id)
{
	switch(id)
	{
	case I_PIECE:
		g_player.x = 2;
		g_player.y = 19;
		g_player.piece.a = (uint8_t (*)[4][4][4])&I_piece;
		g_player.rotation = 0;
		g_player.type = id;
		break;
	case J_PIECE:
		g_player.x = 2;
		g_player.y = 19;
		g_player.piece.b = (uint8_t (*)[4][3][3])&J_piece;
		g_player.rotation = 0;
		g_player.type = id;
		break;
	}
}
