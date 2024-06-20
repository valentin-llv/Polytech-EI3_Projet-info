#include <stdio.h>
#include <stdlib.h>

#include "../../lib/snakeAPI/snakeAPI.h"

#include "init.h"
#include "../structs/structs.h"

extern int whoPlay;

void initGame(GameData* gameData) {
    char* gameName = malloc(sizeof(char) * 50);
    int nbWalls, sizeX, sizeY;

    // Get game infos
    // TRAINING SUPER_PLAYER difficulty=2
    waitForSnakeGame("TOURNAMENT EI", gameName, &sizeX, &sizeY, &nbWalls);
    printf("Game name: %s, nbWalls: %d, sizeX: %d, sizeY: %d\n", gameName, nbWalls, gameData->sizeX, gameData->sizeY);

    // Skrink memory used by vars
    gameData->sizeX = (unsigned char) sizeX;
    gameData->sizeY = (unsigned char) sizeY;

    // Get map infos
    int* walls = (int *) malloc(sizeof(int) * nbWalls * 4);
    whoPlay = getSnakeArena(walls);

    // Init 2D array for map
    gameData->map = allocGameMap(gameData->sizeX, gameData->sizeY);
    fillMap(gameData, nbWalls, walls);

    // Calc user snake and oponent snake positions
    gameData->us.x = (whoPlay) ? sizeX - 3 : 2;
    gameData->us.y = sizeY >> 1;

    gameData->ust.x = gameData->us.x;
    gameData->ust.y = gameData->us.y;

    gameData->os.x = (!whoPlay) ? sizeX - 3 : 2;
    gameData->os.y = gameData->us.y;
    
    // Update map info with snake positions data
    gameData->map[gameData->us.x][gameData->us.y] |= (1 << 7) + (1 << 4);
    gameData->map[gameData->os.x][gameData->os.y] |= (1 << 4);

    // Pre fill cmd with empty lines
    // for(int i = 0; i < (gameData->sizeY + (gameData->sizeY - 1) + 2) * 2 - 1; i++) printf("\n");
}

void fillMap(GameData* gameData, int nbWalls, int* walls) {
    // Pre-fill map with exterior walls
    for(unsigned char i = 0; i < gameData->sizeX; i++) {
        for(unsigned char j = 0; j < gameData->sizeY; j++) {
            if(i == 0) gameData->map[i][j] |= 1 << 3;
            if(i == gameData->sizeX - 1) gameData->map[i][j] |= 1 << 1;

            if(j == 0) gameData->map[i][j] |= 1 << 0;
            if(j == gameData->sizeY - 1) gameData->map[i][j] |= 1 << 2;
        }
    }

    // Fill map with given walls
    for(int i = 0; i < nbWalls * 4; i += 4) {
        if(walls[i + 0] < walls[i + 2]) {
            gameData->map[walls[i + 0]][walls[i + 1]] |= 1 << 1;
            gameData->map[walls[i + 2]][walls[i + 1]] |= 1 << 3;
        } else if(walls[i + 0] > walls[i + 2]) {
            gameData->map[walls[i + 0]][walls[i + 1]] |= 1 << 3;
            gameData->map[walls[i + 2]][walls[i + 1]] |= 1 << 1;
        } else if(walls[i + 1] < walls[i + 3]) {
            gameData->map[walls[i + 0]][walls[i + 1]] |= 1 << 2;
            gameData->map[walls[i + 0]][walls[i + 3]] |= 1 << 0;
        } else {
            gameData->map[walls[i + 0]][walls[i + 1]] |= 1 << 0;
            gameData->map[walls[i + 0]][walls[i + 3]] |= 1 << 2;
        }
    }
}