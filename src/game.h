#ifndef GAME_H
#define GAME_H

#define GRIDSZX 10
#define GRIDSZY 22
#define BLOCKSIZE 24

#define TIMERDECRATE (.25)
#define ROWSPERLEVEL 3

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

typedef struct grabbag
{
	unsigned int	pieces[7];
	unsigned int	index;
	unsigned int	npiece;
} grabbag_t;

extern player_info_t g_player;
extern grabbag_t g_grabbag;

enum
{
	NONE  = 0,
	LEFT  = 1,
	RIGHT = 2
};

typedef struct game_info
{
	int		gamestate;
	bool		running;/* Is the game running? */
	unsigned int    rows_cleared;
	unsigned int	score;
	unsigned int	level;
} game_info_t;

extern uint8_t g_blockgrid[GRIDSZX][GRIDSZY];
extern game_info_t g_game;

uint32_t delay_for_level(uint32_t);
int get_next_piece(void);
void regenerate_bag(void);
void handle_scoring(uint32_t);

#endif
