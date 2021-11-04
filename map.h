#ifndef GAME_MAP
#define GAME_MAP

void debugLog(char* s);

#include "enemy.h"
#include "player.h"

typedef struct{
	__int8_t m_gFlags; /*game status*/
	int m_height, m_width;
	Player m_player;
	Enemy m_enemy;
	Vector2 m_bullet;
	Tank* shooter;
	char** m_mapVisuals;
} Map;

Map* map_init(
	const int mapHeight, const int mapWidth,		 		/*Map*/
	const int playerX,   const int playerY,  int playerOri,	/*Player*/
	const int enemyX,    const int enemyY,   int enemyOri	/*Enemy*/
);

void map_destroy(Map* map);
void map_update(Map* map);

#endif
