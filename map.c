#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "map.h"
#include "log.h"

void map_draw(const Map* map);
void map_fetchInput(Map* map);
void map_updateVisuals(Map* map);
void map_spawnBullet(Map* map, Tank* shooter);
void map_enableInput(Map* map);
void map_disableInput(Map* map);
void map_checkLoseCondition(Map* map);
void map_checkWinCondition(Map* map);
void map_updateBullet(Map* map);
void map_updatePlayer(Map* map, char action);
void map_damage(Map* map, Tank* tank);

Map* map_init(
		const int mapHeight, const int mapWidth,		 		/*Map*/
		const int playerX,   const int playerY,  int playerOri,	/*Player*/
		const int enemyX,    const int enemyY,   int enemyOri	/*Enemy*/
){
	/*allocate heap memory for the map*/
	Map* this = (Map*) malloc(sizeof(Map));
	
	/*Map stuff:*/
	this->m_gFlags = 0b01000001;
	this->m_height = mapHeight;
	this->m_width  = mapWidth;
	
	/*allocate heap memory for the 2D map:*/
	this->m_mapVisuals = (char**)malloc((mapHeight*sizeof(char *))
							  +(mapHeight*mapWidth*sizeof(char)));
	
	char* ptr = (char*)(this->m_mapVisuals + mapHeight);
	for(int i=0; i < mapHeight; ++i)
		this->m_mapVisuals[i] = ptr + mapWidth*i;
	
	/*Player stuff*/
	tank_init(&this->m_player.tank, 1, playerY, playerX, playerOri);
	
	/*Enemy stuff*/
	tank_init(&this->m_enemy.tank, 3, enemyY, enemyX, enemyOri);
	
	return this;
}

void map_destroy(Map* map){
	DEBUG_LOG("Deleting map Visuals...");
	free(map->m_mapVisuals);
	DEBUG_LOG("Deleting the Map...");
	free(map);
}

void map_update(Map* map){
	DEBUG_LOG("Updating Visuals...");
	map_updateVisuals(map);
	
	DEBUG_LOG("Drawing Map...");
	map_draw(map);
	
	DEBUG_LOG("Fetching Input...");
	map_fetchInput(map);
	
	DEBUG_LOG("Updating Bullet...");
	map_updateBullet(map);
}

void map_draw(const Map* map){
#if DEBUG < 1
	puts("\n");
	system("clear");
#endif
	for(int x=0; x < map->m_width; ++x){
		for(int y=0; y < map->m_height; ++y){
			if(map->m_mapVisuals[x][y] == '-'){
				printf("\033[;31m-\033[0m");
			}else if(map->m_mapVisuals[x][y] == '|'){
				printf("\033[;31m|\033[0m");
			}else{
				putc(map->m_mapVisuals[x][y], stdout);
			}
		}
		putc('\n', stdout);
	}
}

void map_fetchInput(Map* map){
	/*Check if bullet is fired*/
	if(map->m_gFlags & 0b00000010) return;

	char input;
	puts("w to go/face up");
	puts("s to go/face down");
	puts("a to go/face left");
	puts("d to go/face right");
	puts("f to shoot laser");
	printf("action:");
	scanf("%c", &input);
		
	switch(input){
		case 'w':
		case 'a':
		case 's':
		case 'd':
			map_updatePlayer(map, input);
			break;
		case 'f':
			map_spawnBullet(map, &map->m_player.tank);
			break;
	}
}

void map_updateVisuals(Map* map){
	for(int x=0; x < map->m_width; ++x){
		for(int y=0; y < map->m_height; ++y){
			/*Draw outline:*/
			if((!x) || (!y) 
				|| (x == (map->m_width-1))
				|| (y == (map->m_height-1))
			){
				DEBUG_LOG("Drawing Outline...");
				map->m_mapVisuals[x][y] = '*';
				DEBUG_LOG("Drawn Outline!");
				continue;
			}
			
			/*Check if this is the players position*/
			if(map->m_player.tank.position.x == x && map->m_player.tank.position.y == y){
				DEBUG_LOG("Drawing Player...");
				if(map->m_player.tank.rotation == 'l') map->m_mapVisuals[x][y] = '<';
				else if(map->m_player.tank.rotation == 'r') map->m_mapVisuals[x][y] = '>';
				else if(map->m_player.tank.rotation == 'u') map->m_mapVisuals[x][y] = '^';
				else if(map->m_player.tank.rotation == 'd') map->m_mapVisuals[x][y] = 'v';
				else map->m_mapVisuals[x][y] = 'P';
				continue;
			}
			
			/*Check if this is the enemys position*/
			if(map->m_enemy.tank.position.x == x && map->m_enemy.tank.position.y == y){
				DEBUG_LOG("Drawing Enemy...");
				if(map->m_enemy.tank.rotation == 'l') map->m_mapVisuals[x][y] = '<';
				else if(map->m_enemy.tank.rotation == 'r') map->m_mapVisuals[x][y] = '>';
				else if(map->m_enemy.tank.rotation == 'u') map->m_mapVisuals[x][y] = '^';
				else if(map->m_enemy.tank.rotation == 'd') map->m_mapVisuals[x][y] = 'v';
				else map->m_mapVisuals[x][y] = 'E';
				continue;
			}
			
			/*Check if bullet is fired*/
			if(map->m_gFlags & 0b00000010){
				DEBUG_LOG("Drawing Bullet...");
				/*Check if its the bullets position:*/
				if(map->m_bullet.x == x && map->m_bullet.y == y){
					if((map->shooter->rotation == 'u')
					 ||(map->shooter->rotation == 'd')
					) map->m_mapVisuals[x][y] = '|';
					else map->m_mapVisuals[x][y] = '-';
					continue;
				}
			}
			
			/*default character*/
			map->m_mapVisuals[x][y] = ' ';
		}
	}
}

void map_spawnBullet(Map* map, Tank* shooter){
	map->m_bullet.x = shooter->position.x;
	map->m_bullet.y = shooter->position.y;
	map->shooter = shooter;
	map_disableInput(map);
}

void map_enableInput(Map* map){
	map->m_gFlags &= ~0b00000010;
}

void map_disableInput(Map* map){
	map->m_gFlags |= 0b00000010;
}

void map_updateBullet(Map* map){
	if(!(map->m_gFlags & 0b00000010)) return;
	
	sleep(1);
	
	if(map->shooter->rotation == 'l'){ 
		--map->m_bullet.y;
		if(map->m_bullet.y < 0){ 
			map_enableInput(map);
			return;
		}
	} else if(map->shooter->rotation == 'r'){
		++map->m_bullet.y;
		if(map->m_bullet.y > map->m_width - 2){ 
			map_enableInput(map);
			return;
		}
	} else if(map->shooter->rotation == 'u'){ 
		--map->m_bullet.x;
		if(map->m_bullet.x < 0){ 
			map_enableInput(map);
			return;
		}
	}else if(map->shooter->rotation == 'd'){ 
		++map->m_bullet.x;
		if(map->m_bullet.x > map->m_height - 2){ 
			map_enableInput(map);
			return;
		}
	}
	
	/*Check collision with enemy*/
	if(map->m_bullet.x == map->m_enemy.tank.position.x
	&& map->m_bullet.y == map->m_enemy.tank.position.y){
		map_damage(map, &map->m_enemy.tank);
		map_enableInput(map);
		return;
	}
	
	/*Check collision with player*/
	if(map->m_bullet.x == map->m_player.tank.position.x
	&& map->m_bullet.y == map->m_player.tank.position.y){
		map_damage(map, &map->m_player.tank);
		map_enableInput(map);
		return;
	}
}

void map_damage(Map* map, Tank* tank){
	tank_damage(tank);
	map_checkWinCondition(map);
	map_checkLoseCondition(map);
}

void map_updatePlayer(Map* map, char action){
	char direction;
	switch(action){
		case 'f':
			map_spawnBullet(map, &map->m_player.tank);
			return;
			break;
		case 'w':
			direction = 'u';
			if(map->m_player.tank.position.x-1 < 1){ 
				map->m_player.tank.rotation = direction;
				return;
			}
			if(map->m_player.tank.position.x-1 == map->m_enemy.tank.position.x
			&& map->m_player.tank.position.y == map->m_enemy.tank.position.y
			){ 
				map->m_player.tank.rotation = direction;
				return;
			}
			
			break;
		case 'a':
			direction = 'l';
			if(map->m_player.tank.position.y-1 < 1){
				map->m_player.tank.rotation = direction;
				return;
			}
			if(map->m_player.tank.position.y-1 == map->m_enemy.tank.position.y
			&& map->m_player.tank.position.x == map->m_enemy.tank.position.x
			){ 
				map->m_player.tank.rotation = direction;
				return;
			}
			
			break;
		case 's':
			direction = 'd';
			if(map->m_player.tank.position.x+2 > (map->m_height-1)){
				map->m_player.tank.rotation = direction;
				return;
			}
			if(map->m_player.tank.position.x+1 == map->m_enemy.tank.position.x
			&& map->m_player.tank.position.y == map->m_enemy.tank.position.y
			){ 
				map->m_player.tank.rotation = direction;
				return;
			}
			
			break;
		case 'd':
			direction = 'r';
			if(map->m_player.tank.position.y+2 > (map->m_width-1)){
				map->m_player.tank.rotation = direction;
				return;
			}
			if(map->m_player.tank.position.y+1 == map->m_enemy.tank.position.y
			&& map->m_player.tank.position.x == map->m_enemy.tank.position.x
			){ 
				map->m_player.tank.rotation = direction;
				return;
			}
			
			break;
		break;
	}
	tank_move(&map->m_player.tank, direction);
	if(enemy_aggro(&map->m_enemy, &map->m_player.tank)){
		map_spawnBullet(map, &map->m_enemy.tank);
	}
}

void map_checkWinCondition(Map* map){
	if(map->m_enemy.tank.m_health > 0) return;
	map_updateVisuals(map);
	int x = map->m_enemy.tank.position.x;
	int y = map->m_enemy.tank.position.y;
	map->m_mapVisuals[x][y] = 'X';
	map_disableInput(map);
	map_draw(map);
	puts("\033[;32mYOU WON!\033[0m");
	map->m_gFlags &= ~0b00000001;
}

void map_checkLoseCondition(Map* map){
	if(map->m_player.tank.m_health > 0) return;
	map_updateVisuals(map);
	int x = map->m_player.tank.position.x;
	int y = map->m_player.tank.position.y;
	map->m_mapVisuals[x][y] = 'X';
	map_disableInput(map);
	map_draw(map);
	puts("\033[;31mYOU LOST!\033[0m");
	map->m_gFlags &= ~0b01000001;
}
