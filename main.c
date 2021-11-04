#include <stdio.h>
#include <stdlib.h>

/*Possible return values:*/
#define STATUS_SUCCESS 0
#define STATUS_EXCEPTION -1

#define DEBUG 0
#include "map.h"
#include "log.h"

/*__attribute__((noreturn)) inline*/ 
void throwUsageException(const char* progName);

void debugLog(char* s){
	printf("DEBUG_LOG: %s\n", s);
}

int main(int argc, char *argv[]){
	DEBUG_LOG("Application started!");
	/*Check Args length:*/
	if(argc < 9){
		DEBUG_LOG("Too less arguments provided!");
		throwUsageException(argv[0]);
	}
	
	/*Parse Args:*/
	DEBUG_LOG("Parsing Arguments...");
	int mapHeight 	= atoi(argv[1]);
	int mapWidth 	= atoi(argv[2]);
	int playerX 	= atoi(argv[3]);
	int playerY 	= atoi(argv[4]);
	int playerOri 	= *argv[5];
	int enemyX 		= atoi(argv[6]);
	int enemyY 		= atoi(argv[7]);
	int enemyOri 	= *argv[8];
	DEBUG_LOG("Arguments parsed!");
	
	/*sanity checks*/
	DEBUG_LOG("Checking Map...");
	if(mapHeight > 25) throwUsageException(argv[0]);
	if(mapWidth > 25) throwUsageException(argv[0]);
	if(mapHeight < 5) throwUsageException(argv[0]);
	if(mapWidth < 5) throwUsageException(argv[0]);
	
	DEBUG_LOG("Checking Player...");
	if(playerX > (mapWidth-2)) throwUsageException(argv[0]);
	if(playerY > (mapHeight-2)) throwUsageException(argv[0]);
	if(playerY < 0) throwUsageException(argv[0]);
	if(playerX < 0) throwUsageException(argv[0]);
	
	switch(playerOri){
		case 'l':
		case 'r':
		case 'u':
		case 'd':
			break;
		default:
			throwUsageException(argv[0]);
			break;
	}
	
	DEBUG_LOG("Checking Enemy...");
	if(enemyX > (mapWidth-2)) throwUsageException(argv[0]);
	if(enemyY > (mapHeight-2)) throwUsageException(argv[0]);
	if(enemyY < 0) throwUsageException(argv[0]);
	if(enemyX < 0) throwUsageException(argv[0]);

	switch(enemyOri){
		case 'l':
		case 'r':
		case 'u':
		case 'd':
			break;
		default:
			throwUsageException(argv[0]);
			break;
	}
	
	if((enemyX == playerX) && (enemyY == playerY))
		throwUsageException(argv[0]);
	DEBUG_LOG("sanity Checks passed!");
	
	/*Construct Game Map:*/
	DEBUG_LOG("Constructing Map...");
	Map* map = map_init(
		mapHeight, mapWidth,		 /*Map*/
		playerX, playerY, playerOri, /*Player*/
		enemyX, enemyY, enemyOri	 /*Enemy*/
	);
	DEBUG_LOG("Map constructed!");
	
	DEBUG_LOG("Starting Game...");
	/*Game Loop:*/
	while(map->m_gFlags&1){
		map_update(map);
	}
	
	/*Cleanup:*/
	map_destroy(map);
    
    return STATUS_SUCCESS;
}

void throwUsageException(const char* progName){
	puts("\033[1;33m-------------------------------------");
	puts("-------\033[;31mUSAGE-EXCEPTION-THROWN!\033[0m\033[1;33m-------");
	puts("-------------------------------------");
	puts("\033[1;32mUsage Information:");
	puts("\033[1;33m-------------------------------------");
	printf("\033[1;35m%s \033[0;36m<mapHeight> \033[0;34m<mapWidth> \033[0;37m<playerX> \033[1;36m<playerY> \033[1;33m<playerOrientation> \033[1;34m<enemyX> \033[0;31m<enemyY> \033[1;32m<enemyOrientation>\n",progName);
	puts("\033[1;33m-------------------------------------");
	puts("\033[0;36mmapHeight:\tInteger Value between 5 and 25 (inclusive)");
	puts("\033[0;34mmapWidth:\tInteger Value between 5 and 25 (inclusive)");
	puts("\033[0;37mplayerX:\tPositive integer Value smaller than mapWidth");
	puts("\033[1;36mplayerY:\tPositive integer Value smaller than mapHeight");
	puts("\033[1;33mplayerOrientation:\n\tr - right\n\tl - left\n\tu - up\n\td - down");
	puts("\033[1;34menemyX:\tPositive integer Value smaller than mapWidth");
	puts("\033[0;31menemyY:\tPositive integer Value smaller than mapHeight");
	puts("\033[1;32menemyOrientation:\n\tr - right\n\tl - left\n\tu - up\n\td - down");
	puts("\033[1;33m-------------------------------------");
	puts("\033[;31mNote: \033[1;34mplayer and enemy position can't be the same!");
	puts("\033[1;33m-------------------------------------");
	puts("\033[0m");
	exit(STATUS_EXCEPTION);
}

