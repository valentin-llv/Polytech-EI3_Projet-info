#include <stdio.h>
#include <stdlib.h>

#include "../../lib/snakeAPI/snakeAPI.h"

#include "snake.h"
#include "../structs/structs.h"

unsigned char moveSnake(GameData* gameData, unsigned char player, unsigned char move, unsigned char grow) {
    // Select wich player to move
    Coord* headPos = player ? &gameData->us : &gameData->os;
    unsigned char dir;

    // Verify move and update head position and save dir t next body position
    if(move == 0 && headPos->y > 0 && (gameData->map[headPos->x][headPos->y] & (1 << 0)) == 0 && ((gameData->map[headPos->x][headPos->y - 1] >> 4) & 7) == 0) {
        headPos->y -= 1;
        dir = 2;
    } else if(move == 1 && headPos->x < gameData->sizeX - 1 && (gameData->map[headPos->x][headPos->y] & (1 << 1)) == 0 && ((gameData->map[headPos->x + 1][headPos->y] >> 4) & 7) == 0) {
        headPos->x += 1;
        dir = 3;
    } else if(move == 2 && headPos->y < gameData->sizeY - 1 && (gameData->map[headPos->x][headPos->y] & (1 << 2)) == 0 && ((gameData->map[headPos->x][headPos->y + 1] >> 4) & 7) == 0) {
        headPos->y += 1;
        dir = 0;
    } else if(move == 3 && headPos->x > 0 && (gameData->map[headPos->x][headPos->y] & (1 << 3)) == 0 && ((gameData->map[headPos->x - 1][headPos->y] >> 4) & 7) == 0) {
        headPos->x -= 1;
        dir = 1;
    } else return 0;

    // Fill map coord with snake data
    gameData->map[headPos->x][headPos->y] = (((player << 3) + (2 + dir)) << 4) + (gameData->map[headPos->x][headPos->y] & 15);
    
    if(!grow) deleteTail(gameData, headPos->x, headPos->y);
    return 1;
}

void deleteTail(GameData* gameData, char x, char y) {
    unsigned char newX = x, newY = y;
    unsigned char nextBodyDir = ((gameData->map[x][y] >> 4) & 7) - 2;

    // Can use switch for slightly better performance
    if(nextBodyDir == 0) newY -= 1;
    else if(nextBodyDir == 1) newX += 1;
    else if(nextBodyDir == 2) newY += 1;
    else if(nextBodyDir == 3) newX -= 1;

    if(((gameData->map[newX][newY] >> 4) & 7) == 1) {
        gameData->map[newX][newY] &= 15;
        gameData->map[x][y] = (gameData->map[x][y] & (1 << 7)) + (1 << 4) + (gameData->map[x][y] & 15);

        gameData->ust.x = x;
        gameData->ust.y = y;
    } else deleteTail(gameData, newX, newY);
}