#ifndef INIT_H
#define INIT_H

#include "../structs/structs.h"

void initGame(GameData* gameData);
void fillMap(GameData* gameData, int nbWalls, int* walls);

#endif