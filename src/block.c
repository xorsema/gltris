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

/* Return a proper piece_ptr_t for the given type of piece, which contains the data*/
piece_ptr_t block_pointer_from_type(int type)
{
	piece_ptr_t r;

	switch(type)
	{
	case I_PIECE:
		r.a = (uint8_t (*)[4][4][4])&I_piece;
		break;
	case O_PIECE:
		r.a = (uint8_t (*)[4][4][4])&O_piece;
		break;
	case J_PIECE:
		r.b = (uint8_t (*)[4][3][3])&J_piece;
		break;
	case L_PIECE:
		r.b = (uint8_t (*)[4][3][3])&L_piece;
		break;
	case S_PIECE:
		r.b = (uint8_t (*)[4][3][3])&S_piece;
		break;
	case T_PIECE:
		r.b = (uint8_t (*)[4][3][3])&T_piece;
		break;
	case Z_PIECE:
		r.b = (uint8_t (*)[4][3][3])&Z_piece;
		break;
	}
	return r;
}

/* Check collisions with the single block which is part of a piece */
int check_piece_block_collision(int px, int py, int bx, int by, int type, int rot)
{
	int		x, y;
	int		b;
	piece_ptr_t	ptr;
	int		r;

	x   = px + bx;
	y   = py + by;
	ptr = block_pointer_from_type(type);
	b   = get_block(bx, by, rot, type, ptr);
	r   = NO_COLLISION;

	/* Don't bother when there's no block */
	if(b == 0)
		return NO_COLLISION;

	/* Check collision with the walls */
	if(x > (GRIDSZX - 1))
		r |= COLLISION_RWALL;
	else  if(x < 0)
		r |= COLLISION_LWALL;

	/* Check if it's touching the floor (but has not gone past it) */
	if(y == 0)
		r |= TOUCHING_FLOOR;
	
	/* Check collision with the floor */
	if(y < 0)
		r |= COLLISION_FLOOR;
	
	/* Check collision with a placed block */
	if((y < GRIDSZY) && (x < GRIDSZX) && g_blockgrid[x][y] != 0)
		r |= COLLISION_BLOCK;			
	
	/* Check if the block is above the ceiling (-1 for being zero indexed and -2 for the 2 rows above the ceiling */
	if(y > GRIDSZY-3)
		r |= COLLISION_CEILING;
	
	return r;
}

/* Check for collisions with every block in the player's piece, returning NO_COLLISION if none have been found */
int check_collisions(int inx, int iny, unsigned int inrot)
{
	int	i, j;
	int	max; 
	int	b;
	int	r;

	if(g_player.type == NULL_PIECE)
		return NO_COLLISION;	
	
	max = get_piece_size(g_player.type);
	r   = NO_COLLISION;

	for(i = 0; i < max; i++)
	{
		for(j = 0; j < max; j++)
		{
			r |= check_piece_block_collision(inx, iny, i, j, g_player.type, inrot);
		}
	}
	return r;
}

/* Writes the player piece to the grid */
void set_piece(void)
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

/* Check for any full rows, return the max and min as two 16bit ints in one 32 bit int */
unsigned int check_rows(void)
{
	int			i, j;
	unsigned int		result;
	unsigned short int	max, min;
	bool			fr;
	bool			nfr = true;
	bool			ms  = false;

	for(j = 0; j < GRIDSZY; j++)
	{
		for(i = 0, fr = true; i < GRIDSZX; i++)
		{
			if(g_blockgrid[i][j] == 0){
				fr = false;
				break;
			}
		}

		if(fr == true){
			max = j;
			nfr = false;
		}

		if(ms != true && fr == true){
			min = j;
			ms = true;
		}
	}

	/* If there were no full rows, return a constant to indicate failure */
	if(nfr == true)
		result = NOFULLROWS;
	else {
                /* Store the max as the high 16 bits of result, and the min as the low 16 bits */
		result = (max << 16);
		result += (min & 0xFFFF);
	}
	return result;
}

/* Clear all rows from low to high */
void clear_rows(unsigned int high, unsigned int low)
{
	int i, j;

	for(j = low; j != (high+1); j++)
	{
		for(i = 0; i < GRIDSZX; i++)
		{
			g_blockgrid[i][j] = 0;
		}
	}
}

/* Shift all rows from target up down by offset */
void shift_rows(unsigned int target, unsigned int offset)
{
	int		i, j;
	unsigned int	tb;

	for(j = target; j < GRIDSZY; j++)
	{
		for(i = 0; i < GRIDSZX; i++)
		{
			tb = g_blockgrid[i][j];
			g_blockgrid[i][j] = 0;
			g_blockgrid[i][j-offset] = tb;
		}
	}
}

/* If any rows have been completely filled, clear them, then move down the other blocks */
void handle_clearance(void)
{
	unsigned int		r;
	unsigned short int	max, min;

	r   = check_rows();
	max = (r >> 16);
	min = r & 0xFFFF;

	if(r != NOFULLROWS){
		clear_rows(max, min);
		shift_rows(max+1, (max+1) - min);
		handle_scoring((max+1) - min);
	}	
}

/* Check if the game should be reset (if all the blocks of the player piece are in the ceiling) */
bool check_gameover(void)
{
	int	i, j;
	int	c;

	for(i = 0; i < PBLOCKMAX; i++)
	{
		for(j = 0; j < PBLOCKMAX; j++)
		{
			c = check_piece_block_collision(g_player.x, g_player.y, i, j, g_player.type, g_player.rotation);
			if(!(c & COLLISION_CEILING))
				return false;
		}
	}

	return true;
}

void do_reset(void)
{
	spawn_piece(get_piece());
	g_game.level = 0;
	g_game.rows_cleared = 0;
	modify_timer(g_fall_timer, STARTINGTIME);
}

/* Make sure we can set the piece, then set it, or reset the game */
void handle_placement(void)
{
	if(check_gameover()){
		clear_rows(GRIDSZY-1, 0);
		g_game.gamestate = STATE_GAMEOVER;
		return;
	}
	set_piece();
	g_player.type = NULL_PIECE;
	g_player.piece.a = NULL;
	handle_clearance();
	modify_timer(g_fall_timer, delay_for_level(g_game.level));
}

/* Handle all movement of the player piece */
void do_movement(void)
{
	int c;

	if(g_player.type != NULL_PIECE){
		/* If the button to snap the piece down was pressed, move it down until it collides with something, and then place it */
		if(g_player.snap == true){
			while(!(check_collisions(g_player.x, g_player.y, g_player.rotation) & TOUCHING_FLOOR) && !(check_collisions(g_player.x, g_player.y-1, g_player.rotation) & COLLISION_BLOCK))
			{
				g_player.y -= 1;
			}
			g_player.snap = false;
			handle_placement();
		}

		/* No point in rotating a perfectly square piece, just set this to none */
		if(g_player.type == O_PIECE){
			g_player.rotate = NONE;
		}

		/* Move the piece left if the correct button has been pressed */
		if(g_player.move == LEFT){
			c = check_collisions(g_player.x - 1, g_player.y, g_player.rotation);
			if(!(c & COLLISION_BLOCK) && !(c & COLLISION_LWALL)){
				g_player.x -= 1;
			}
			g_player.move = NONE;
		}

		/* Do the same with the right */
		if(g_player.move == RIGHT){
			c = check_collisions(g_player.x + 1, g_player.y, g_player.rotation);
			if(!(c & COLLISION_BLOCK) && !(c & COLLISION_RWALL)){
				g_player.x += 1;
			}
			g_player.move = NONE;
		}
	
		/* Apply "gravity" to the piece, if the timer has elapsed, if the piece is as far down as possible, place it */
		if(g_fall_timer->elapsed == true){/* If on this frame we should move the piece down and there haven't been any collisions between the piece and a block/the ground */
			if(!(check_collisions(g_player.x, g_player.y, g_player.rotation) & TOUCHING_FLOOR) && !(check_collisions(g_player.x, g_player.y-1, g_player.rotation) & COLLISION_BLOCK))
				g_player.y -= 1;
			else 
				handle_placement();
		}
	}
}

void set_rotation(int rotation)
{
	int	c;
	int	x, y;
	
	x = g_player.x;
	y = g_player.y;

	/* Get new coordinates with the given rotation that aren't in the wall or the floor */
	for(c = check_collisions(x, y, rotation); (c & COLLISION_RWALL) || (c & COLLISION_LWALL) || (c & COLLISION_FLOOR); c = check_collisions(x, y, rotation))
	{
		if(c & COLLISION_RWALL)
			x -= 1;
		if(c & COLLISION_LWALL)
			x += 1;
		if(c & COLLISION_FLOOR)
			y += 1;
	}

	/* If the coordinates are also not colliding with any blocks, they are valid, so set them */
	if(!(c & COLLISION_BLOCK)){
		g_player.x	  = x;
		g_player.y	  = y;
		g_player.rotation = rotation;
	}
}

/* Rotate our player piece if g_player.rotate is set, and rotating is valid */
void do_rotation(void)
{
	int r;

	switch(g_player.rotate)
	{
	case NONE:
		break;
	case RIGHT:
	{
		r = (g_player.rotation == 3) ? 0 : g_player.rotation + 1;
		set_rotation(r);
		g_player.rotate = NONE;
		break;
	}
	case LEFT:
	{
		r = (g_player.rotation == 0) ? 3 : g_player.rotation - 1;
		set_rotation(r);
		g_player.rotate = NONE;
		break;
	}
	}
}

void get_ghost_info(int *ox, int *oy)
{
	 int x, y;
	
	x = g_player.x;
	y = g_player.y;

	while(!(check_collisions(x, y, g_player.rotation) & TOUCHING_FLOOR) && !(check_collisions(x, y-1, g_player.rotation) & COLLISION_BLOCK))
	{
		y -= 1;
	}
	
	*ox = x;
	*oy = y;
}

/* Take care of piece/block mechanics */
void handle_blocks(void)
{
	if(g_player.type == NULL_PIECE)
		spawn_piece(get_piece());

	do_movement();
	do_rotation();
}

/* Give the player a new piece */
void spawn_piece(unsigned int id)
{
	g_player.rotation = 0;
	g_player.type	  = id;
	g_player.x	  = 4;
	g_player.y	  = 20;
	g_player.rotate	  = NONE;
	g_player.move	  = NONE;
	g_player.snap	  = false;
	g_player.piece	  = block_pointer_from_type(g_player.type);

	if(check_collisions(g_player.x, g_player.y, g_player.rotation) & COLLISION_BLOCK){
		clear_rows(GRIDSZY-1, 0);
		g_game.gamestate = STATE_GAMEOVER;
	}
}
