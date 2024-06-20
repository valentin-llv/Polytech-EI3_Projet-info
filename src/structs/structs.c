#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

GameData* copyGameData(GameData* source) {
    GameData* dest = (GameData*) malloc(sizeof(GameData));

    unsigned char** map = allocGameMap(source->sizeX, source->sizeY);

    for(unsigned char i = 0; i < source->sizeX; i++) {
        for(unsigned char j = 0; j < source->sizeY; j++) map[i][j] = source->map[i][j];
    }

    dest->map = map;

    dest->sizeX = source->sizeX;
    dest->sizeY = source->sizeY;

    dest->us.x = source->us.x;
    dest->us.y = source->us.y;

    dest->ust.x = source->ust.x;
    dest->ust.y = source->ust.y;

    dest->os.x = source->os.x;
    dest->os.y = source->os.y;

    return dest;
}

unsigned char** allocGameMap(unsigned char sizeX, unsigned char sizeY) {
    unsigned char** map = (unsigned char **) malloc(sizeof(unsigned char *) * sizeX);
    for(unsigned char i = 0; i < sizeX; i++) {
        map[i] = (unsigned char *) malloc(sizeof(unsigned char) * sizeY);
        for(unsigned char j = 0; j < sizeY; j++) map[i][j] = 0;
    }

    return map;
}

void freeMap(GameData* gameData) {
    for(unsigned char i = 0; i < gameData->sizeX; i++) free(gameData->map[i]);
    free(gameData->map);
}


void freeArray(unsigned char** array, char sizeX) {
    for(unsigned char i = 0; i < sizeX; i++) free(array[i]);
    free(array);
}