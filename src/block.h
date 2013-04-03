#ifndef BLOCK_H
#define BLOCK_H

void offset_block(int, int, int, int);
void handle_blocks(void);
extern const int g_piece_blocks[];

typedef enum collision
{
	NO_COLLISION = 0,
	COLLISION_BELOW = 1,
	COLLISION_LEFT = 2,
	COLLISION_RIGHT = 4
} collision_t;

#endif
