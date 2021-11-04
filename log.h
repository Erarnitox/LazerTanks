#ifndef GAME_LOG
#define GAME_LOG

#if DEBUG > 0
void debugLog(char* s);
#define DEBUG_LOG(...); debugLog(__VA_ARGS__);
#else
#define DEBUG_LOG(...); /*release*/
#endif
#endif
