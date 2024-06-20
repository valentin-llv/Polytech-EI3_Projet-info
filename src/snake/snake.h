#ifndef SNAKE_H
#define SNAKE_H

#include "../structs/structs.h"

unsigned char moveSnake(GameData* gameData, unsigned char player, unsigned char move, unsigned char grow);
void deleteTail(GameData* gameData, char x, char y);

#endif