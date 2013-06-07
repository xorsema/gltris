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

/* Check for collisions, returning NO_COLLISION if none have been found */
int check_collisions(int inx, int iny, unsigned int inrot)
{
	int	i, j;
	int	x, y;
	int	max; 
	int	b;
	int	r;
	bool	isatype;

	if(g_player.type == NULL_PIECE)
		return NO_COLLISION;	

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
			if(x > (GRIDSZX - 1))
				r |= COLLISION_RWALL;
			else  if(x < 0)
				r |= COLLISION_LWALL;

			if(y == 0)
				r |= TOUCHING_FLOOR;
			
			/* Check collision with the floor */
			if(y < 0)
				r |= COLLISION_FLOOR;
			
			/* Check collision with a placed block */
			if((y < GRIDSZY) && (x < GRIDSZX) && g_blockgrid[x][y] != 0)
				r |= COLLISION_BLOCK;			
			
			if(y > GRIDSZY-3)
				r |= COLLISION_CEILING;
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

	r = check_rows();
	max = (r >> 16);
	min = r & 0xFFFF;

	if(r != NOFULLROWS){
		clear_rows(max, min);
		shift_rows(max+1, (max+1) - min);
		handle_scoring((max+1) - min);
	}	
}

void do_reset(void)
{
	clear_rows(GRIDSZY-1, 0);
	spawn_piece(get_piece());
	g_game.level = 0;
	g_game.rows_cleared = 0;
	modify_timer(g_fall_timer, STARTINGTIME);
}

/* Make sure we can set the piece, then set it, or reset the game */
void handle_placement(void)
{
	if(check_collisions(g_player.x, g_player.y, g_player.rotation) & COLLISION_CEILING){
		do_reset();
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
				g_player.move = NONE;
			}
		}

		/* Do the same with the right */
		if(g_player.move == RIGHT){
			c = check_collisions(g_player.x + 1, g_player.y, g_player.rotation);
			if(!(c & COLLISION_BLOCK) && !(c & COLLISION_RWALL)){
				g_player.x += 1;
				g_player.move = NONE;
			}
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

/* Make sure the player piece isn't in the wall or the _in_ floor (touching the floor is fine) */
void fix_position(void)
{
	int c;

	c = check_collisions(g_player.x, g_player.y, g_player.rotation);
	if(c != NO_COLLISION && c != TOUCHING_FLOOR && c != COLLISION_CEILING){
		if(c & COLLISION_RWALL)
			g_player.x -= 1;
		if(c & COLLISION_LWALL)
			g_player.x += 1;
		if(c & COLLISION_FLOOR)
			g_player.y += 1;
		fix_position();		
	}
}

/* Check whether the rotation is valid (if there are no unacceptable collisions) */
bool check_rotation(int rotation)
{
	int c;

	c = check_collisions(g_player.x, g_player.y, rotation);
	if(c != NO_COLLISION && c != TOUCHING_FLOOR && c != COLLISION_CEILING && c != COLLISION_RWALL && c != COLLISION_LWALL)
		return false;

	return true;
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
		g_player.rotation = (check_rotation(r)) ? r : g_player.rotation;
		fix_position();
		g_player.rotate = NONE;
		break;
	}
	case LEFT:
	{
		r = (g_player.rotation == 0) ? 3 : g_player.rotation - 1;
		g_player.rotation = (check_rotation(r)) ? r : g_player.rotation;
		fix_position();
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
	do_movement();
	do_rotation();

	if(g_player.type == NULL_PIECE)
		spawn_piece(get_piece());		
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
	g_player.snap = false;
	g_player.piece = block_pointer_from_type(g_player.type);
}
