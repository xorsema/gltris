#ifndef BLOCK_H
#define BLOCK_H

void offset_block(int, int, int, int);
void handle_blocks(void);
void do_collisions(void);
void offset_player_left(unsigned int);
void offset_player_right(unsigned int);
extern const int g_piece_blocks[];

typedef enum collision
{
	NO_COLLISION = 0,
	COLLISION_BELOW = 1,
	COLLISION_LEFT = 2,
	COLLISION_RIGHT = 4
} collision_t;

extern collision_t g_player_collisions;

#endif
