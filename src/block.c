#include <stdint.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <math.h>

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


/* Check for collisions, returning NO_COLLISION if none have been found */
int check_collisions(int inx, int iny, unsigned int inrot)
{
	int i, j, x, y, max, b, r;
	bool isatype;

	if(g_player.type == NULL_PIECE){
		return NO_COLLISION;
	}

	isatype = (g_player.type == I_PIECE || g_player.type == O_PIECE);/* If it's a type a piece pointer or not */
	max = isatype ? 4 : 3;/* the max size will be 4 or 3 depending on whether it's a 4x4 (a type) or 3x3 (b type) */
	r = NO_COLLISION;

	for(i = 0; i < max; i++)
	{
		x = i + inx;

		for(j = 0; j < max; j++)
		{
			y = j + iny;
						
			b = (isatype) ? (*g_player.piece.a)[inrot][j][i] : (*g_player.piece.b)[inrot][j][i];
			if(b == 0)/* Don't bother checking when it's null block */
				continue;


			/* Check collision with the wall on the x axis (right and left walls) */
			if(x > (GRIDSZX - 1)){
				r |= COLLISION_RWALL;
			} else  if(x < 0){
				r |= COLLISION_LWALL;
			}

			if(y == 0){
				r |= TOUCHING_FLOOR;
			}

			/* Check collision with the floor */
			if(y < 0){
				r |= COLLISION_FLOOR;
			}

			/* Check collision with a placed block */
			if(g_blockgrid[x][y] != 0){
				r |= COLLISION_BLOCK;
			}
		}
	}
	return r;
}

/* Writes the player piece to the grid */
void set_pieces(void)
{
	int i, j;
	for(i = 0; i < PBLOCKMAX; i++)
	{
		for(j = 0; j < PBLOCKMAX; j++)
		{
			int b = get_player_block(i, j);
			if(b != 0){
				g_blockgrid[g_player.x+i][g_player.y+j] = b;
			}
		}
	}
}

/* Handle all movement of the player piece */
void do_movement(void)
{
	if(g_player.type != NULL_PIECE){
		int c;

		if(g_player.move == LEFT){
			c = check_collisions(g_player.x - 1, g_player.y, g_player.rotation);
			if(!(c & COLLISION_BLOCK) && !(c & COLLISION_LWALL)){
				g_player.x -= 1;
				g_player.move = NONE;
			}
		}
		if(g_player.move == RIGHT){
			c = check_collisions(g_player.x + 1, g_player.y, g_player.rotation);
			if(!(c & COLLISION_BLOCK) && !(c & COLLISION_RWALL)){
				g_player.x += 1;
				g_player.move = NONE;
			}
		}
	
		if(g_second_timer->elapsed == true){/* If this frame falls on a second mark and there haven't been any collisions between the piece and a block/the ground */
			if(!(check_collisions(g_player.x, g_player.y, g_player.rotation) & TOUCHING_FLOOR) && !(check_collisions(g_player.x, g_player.y-1, g_player.rotation) & COLLISION_BLOCK)){
				g_player.y -= 1;
			} else {
				set_pieces();
				g_player.type = NULL_PIECE;
				g_player.piece.a = NULL;
			}
		}
	}
}

void fix_position(void)
{
	int c = check_collisions(g_player.x, g_player.y, g_player.rotation);
	if(c != NO_COLLISION && c != TOUCHING_FLOOR){
		if(c & COLLISION_RWALL)
			g_player.x -= 1;
		if(c & COLLISION_LWALL)
			g_player.x += 1;
		if(c & COLLISION_FLOOR)
			g_player.y += 1;
		if(c & COLLISION_BLOCK)
			g_player.y += 1;
		fix_position();		
	}
}

void do_rotation(void)
{
	switch(g_player.rotate)
	{
	case NONE:
		break;
	case RIGHT:
		g_player.rotation = (g_player.rotation == 3) ? 0 : g_player.rotation + 1;
		g_player.rotate = NONE;
		fix_position();
		break;
	case LEFT:
		g_player.rotation = (g_player.rotation == 0) ? 3 : g_player.rotation - 1;
		g_player.rotate = NONE;
		fix_position();
		break;
	}

}

/* Take care of piece/block mechanics */
void handle_blocks(void)
{
	do_movement();
	do_rotation();

	if(g_player.type == NULL_PIECE){
		spawn_piece(S_PIECE);
	}
	
}

/* Give the player a new piece */
void spawn_piece(unsigned int id)
{
	g_player.rotation = 0;
	g_player.type = id;
	g_player.x = 4;
	g_player.y = 20;
	g_player.rotate = NONE;
	g_player.move = NONE;
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
