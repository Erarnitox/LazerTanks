#ifndef GAME_HELPER
#define GAME_HELPER
#include <stdio.h>

void redPaint(char* sentence){
	printf("\033[;31m-\033[0m");
	printf("%s", sentence);
	printf("\033[0m");
}
#endif

