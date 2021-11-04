#ifndef GAME_TANK
#define GAME_TANK

#include "vector2.h"

typedef struct{
	int m_health;
	Vector2 position;
	char rotation;
} Tank;

void tank_move(Tank* this, const char direction);
void tank_damage(Tank* this);
void tank_init(Tank* this, int health, int x, int y, char orientation);
#endif
