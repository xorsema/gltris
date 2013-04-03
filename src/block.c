#include <stdint.h>
#include <stdbool.h>
#include <SDL/SDL.h>

#include "gltris.h"
#include "block.h"
#include "timer.h"

collision_t g_player_collisions;

const int g_piece_blocks[] = {
        /* NULL */        
	0, 0,
	0, 0,
	0, 0,
	0, 0,
        /* I */
	0, 0,
	1, 0,
	2, 0,
	3, 0,
	/* J */
	0, 0,
	1, 0,
	2, 0,
	2, -1,
	/* L */
	0, 0,
	-1, 0,
	1, 0,
	2, 0,
	/* O */
	0, 0,
	1, 0,
	0, 1,
	1, 1
};

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

/* Move the player's block to the right by a specified amount */
void offset_player_right(unsigned int amt)
{
	int i, j;
	for(i = GRIDSZX - 1; i > -1; i--)/* Start from the right so we don't overwrite our own piece */
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			if(g_blockgrid[i][j] > 0){
				offset_block(i, j, amt, 0);
			}
		}
	}
}

/* Move the player's block to the left by a specified amount */
void offset_player_left(unsigned int amt)
{
	int i, j;
	for(i = 0; i < GRIDSZX; i++)
	{
		for(j = 0; j < GRIDSZY; j++)
		{
			if(g_blockgrid[i][j] > 0){
				offset_block(i, j, -(amt), 0);
			}
		}
	}
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
			unsigned int i, j;
			for(i = 0; i < GRIDSZX; i++)
			{
				for(j = 0; j < GRIDSZY; j++)/* loop through the grid and... */
				{
					int8_t t = g_blockgrid[i][j];
					if((t > 0)){/* If it's a player's block (part of a moving piece) */
						offset_block(i, j, 0, -1);/* Apply gravity */
					} 
					
				}
			}
		} else {
			set_pieces();/* "set" the piece's blocks (make them negative) so they aren't counted as a piece anymore */
		}
	}
}

void handle_blocks(void)
{
	do_gravity();
}

void spawn_piece(unsigned int id)
{
	
}
