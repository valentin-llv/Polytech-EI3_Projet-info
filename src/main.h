#ifndef MAIN_H
#define MAIN_H

#include "structs/structs.h"

unsigned char recursiveSearchMove(GameData* gameData, unsigned char depth, unsigned char maxDepth);
void* recursiveThreadedSearchMove(void *arg);

#endif