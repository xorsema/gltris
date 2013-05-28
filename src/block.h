#ifndef BLOCK_H
#define BLOCK_H

#define PBLOCKMAX ((g_player.type == I_PIECE || g_player.type == O_PIECE) ? 4 : 3)
#define get_player_block(x, y) ((g_player.type == I_PIECE || g_player.type == O_PIECE) ? (*g_player.piece.a)[g_player.rotation][y][x] : (*g_player.piece.b)[g_player.rotation][y][x])

void handle_blocks(void);
void do_collisions(void);
void spawn_piece(unsigned int);
void clear_player(void);
void do_wallkicks(void);
int check_collisions(int, int, unsigned int);
int get_next_piece(void);
void regenerate_bag(void);

typedef enum collision
{
	NO_COLLISION	  = 0,
	COLLISION_BLOCK	  = 1,
	COLLISION_LWALL	  = 2,
	COLLISION_RWALL	  = 4,
	COLLISION_FLOOR	  = 8,
	TOUCHING_FLOOR	  = 16,
	COLLISION_CEILING = 32
} collision_t;

enum 
{
	NULL_PIECE = 0,
	I_PIECE	   = 1,
	J_PIECE	   = 2,
	L_PIECE	   = 3,
	O_PIECE	   = 4,
	S_PIECE	   = 5,
	T_PIECE	   = 6,
	Z_PIECE	   = 7
};

typedef union piece_ptr
{
	uint8_t (*a)[4][4][4];
	uint8_t (*b)[4][3][3];
} piece_ptr_t;


#endif
