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

const uint8_t O_piece[4][4][4] = {

	{
		{ 0, 0, 0, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 4, 4, 0 },
		{ 0, 0, 0, 0 }
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

const uint8_t L_piece[4][3][3] = {
	{
		{ 0, 0, 3 },
		{ 3, 3, 3 },
		{ 0, 0, 0 }
	},
	{
		{ 0, 3, 0 },
		{ 0, 3, 0 },
		{ 0, 3, 3 }
	},
	{
		{ 0, 0, 0 },
		{ 3, 3, 3 },
		{ 3, 0, 0 }
	},
	{
		{ 3, 3, 0 },
		{ 0, 3, 0 },
		{ 0, 3, 0 }
	}
};

const uint8_t S_piece[4][3][3] = {
	{
		{ 0, 5, 5 },
		{ 5, 5, 0 },
		{ 0, 0, 0 }
	},
	{
		{ 0, 5, 0 },
		{ 0, 5, 5 },
		{ 0, 0, 5 }
	},
	{
		{ 0, 0, 0 },
		{ 0, 5, 5 },
		{ 5, 5, 0 }
	},
	{
		{ 5, 0, 0 },
		{ 5, 5, 0 },
		{ 0, 5, 0 }
	}
};

const uint8_t T_piece[4][3][3] = {
	{
		{ 0, 6, 0 },
		{ 6, 6, 6 },
		{ 0, 0, 0 }
	},
	{
		{ 0, 6, 0 },
		{ 0, 6, 6 },
		{ 0, 6, 0 }
	},
	{
		{ 0, 0, 0 },
		{ 6, 6, 6 },
		{ 0, 6, 0 }
	},
	{
		{ 0, 6, 0 },
		{ 6, 6, 0 },
		{ 0, 6, 0 }
	}
};

const uint8_t Z_piece[4][3][3] = {
	{
		{ 7, 7, 0 },
		{ 0, 7, 7 },
		{ 0, 0, 0 }
	},
	{
		{ 0, 0, 7 },
		{ 0, 7, 7 },
		{ 0, 7, 0 }
	},
	{
		{ 0, 0, 0 },
		{ 7, 7, 0 },
		{ 0, 7, 7 }
	},
	{
		{ 0, 7, 0 },
		{ 7, 7, 0 },
		{ 7, 0, 0 }
	}
};

/* This basically implements wall kicks */
bool fix_bounds(int x, int y)
{
	if(x < 0){
		g_player.x += abs(x);
		return true;
	}
	if(x > (GRIDSZX - 1)){
		g_player.x -= x - (GRIDSZX - 1);
		return true;
	}
	if(y < 0){
		g_player.y += abs(y);
		return true;
	}
		
		return false;
}

/* Write the player piece to g_blockgrid */
void write_player(void)
{
	int i, j, max;
	int8_t t;
	bool isatype = (g_player.type == I_PIECE || g_player.type == O_PIECE);/* If it's a type a piece pointer or not */
	max = isatype ? 4 : 3;/* If it's a O or I piece it will be 4x4 */
	for(i = 0; i < max; i++)
	{
		int x = i + g_player.x;
		for(j = 0; j < max; j++)
		{
			int y = j + g_player.y;
			t = isatype ? (*g_player.piece.a)[g_player.rotation][j][i] : (*g_player.piece.b)[g_player.rotation][j][i];/* Deref the correct pointer type in the union */

			/* if this isn't a null block and it's outside the range, fix_bounds will return true, so we should redo the writing of our piece */
			if((t != 0) && (fix_bounds(x, y) == true)){
				clear_player();
				write_player();
				return;
			}


			if((t != 0) && g_blockgrid[x][y] == 0){/* If there's nothing there and we're not trying to copy a null block */
				g_blockgrid[x][y] = t;/* Copy our block at the correct position */
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
	g_player.rotation = 0;
	g_player.type = id;
	g_player.x = 2;
	g_player.y = 20;
	switch(id)
	{
	case I_PIECE:
		g_player.piece.a = (uint8_t (*)[4][4][4])&I_piece;
		break;
	case O_PIECE:
		g_player.piece.a = (uint8_t (*)[4][4][4])&O_piece;
		break;
	case J_PIECE:
		g_player.piece.b = (uint8_t (*)[4][3][3])&J_piece;
		break;
	case L_PIECE:
		g_player.piece.b = (uint8_t (*)[4][3][3])&L_piece;
		break;
	case S_PIECE:
		g_player.piece.b = (uint8_t (*)[4][3][3])&S_piece;
		break;
	case T_PIECE:
		g_player.piece.b = (uint8_t (*)[4][3][3])&T_piece;
		break;
	case Z_PIECE:
		g_player.piece.b = (uint8_t (*)[4][3][3])&Z_piece;
		break;
	}
}
