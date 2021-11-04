#ifndef GAME_ENEMY
#define GAME_ENEMY

#include "tank.h"

typedef struct{
	Tank tank;
} Enemy;

int enemy_aggro(Enemy* this, Tank* tank);
#endif

