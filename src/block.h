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
extern const int g_piece_blocks[];

typedef enum collision
{
	NO_COLLISION	= 0,
	COLLISION_BLOCK = 1,
	COLLISION_LWALL = 2,
	COLLISION_RWALL = 4,
	COLLISION_FLOOR = 8,
	TOUCHING_FLOOR	= 16
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

enum
{
	NONE  = 0,
	LEFT  = 1,
	RIGHT = 2
};

typedef union piece_ptr
{
	uint8_t (*a)[4][4][4];
	uint8_t (*b)[4][3][3];
} piece_ptr_t;

typedef struct player_info
{
	int		x, y;	/* X and Y of the origin of the player's piece */
	unsigned int	rotation;	/* Which stage of rotation */
	unsigned int	type;	/* The type id of the player's piece */
	bool		set;	/* true when the piece shouldn't move any more */
	piece_ptr_t	piece;
	int		move;	/* Should we attempt to move left or right? */
	int		rotate; /* Should we attempt to rotate left or right? */
	bool		snap;	/* Should we snap the piece down? */
} player_info_t;

extern player_info_t g_player;

extern collision_t g_player_collisions;

#endif
