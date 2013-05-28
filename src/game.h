#ifndef GAME_H
#define GAME_H

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

#endif
