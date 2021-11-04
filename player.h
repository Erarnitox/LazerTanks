#ifndef GAME_PLAYER
#define GAME_PLAYER

#include "tank.h"

typedef struct{
	Tank tank;
} Player;

void player_update(Player* this);

#endif
