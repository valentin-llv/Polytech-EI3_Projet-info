#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "../lib/snakeAPI/snakeAPI.h"

#include "main.h"
#include "structs/structs.h"
#include "init/init.h"
#include "snake/snake.h"
#include "cui/cui.h"

// Define max search depth
#define MAX_DEPTH 35

// Count number of turn and store who play
int turn = 0;
char whoPlay;

// Search map, used to store already explored paths in order to save computing time by only loonking for unexplored paths
unsigned char** searchMap;

// Function to calc the score of a move
unsigned char calcDir(GameData* gameData, GameData* dataSource, unsigned char dir, unsigned char grow, unsigned char currentScore) {
    // Default score for move is 1
    unsigned char score = 1;

    // Basic check if move available, if not move score is null
    if((gameData->map[gameData->us.x][gameData->us.y] & (1 << dir)) == 0) {
        if(!moveSnake(gameData, 1, dir, grow)) return 0;
    } else return 0;

    // Score modifier 1: Go in the center, better score if the move is towards the center of the map
    if(turn < 100) {
        if((gameData->us.x < (gameData->sizeX >> 1)) && dir == 1) score += 1;
        else if((gameData->us.x > (gameData->sizeX >> 1)) && dir == 3) score += 1;
        if((gameData->us.y < (gameData->sizeY >> 1)) && dir == 2) score += 1;
        else if((gameData->us.y > (gameData->sizeY >> 1)) && dir == 0) score += 1;
    }

    // Score modifier 2: Better score if move go toward ennemy head
    if(turn > 100) { // Is active only after 100 turns
        char distX = abs(dataSource->us.y - dataSource->os.y) - abs(gameData->us.y - gameData->os.y);
        char distY = abs(dataSource->us.y - dataSource->os.y) - abs(gameData->us.y - gameData->os.y);

        if((distX > 0 || distY > 0) && distX >= 0 && distY >= 0) score += 2;
    }

    // If the current move is already in the search map with an higher score the return 0
    if(searchMap[gameData->us.x][gameData->us.y] >= currentScore + score) return 0;
    else searchMap[gameData->us.x][gameData->us.y] = currentScore + score; // Else save

    return currentScore + score;
}

// Function to check to best move in all the fourth possible direction (in fact only 3 but ...)
// Gate state is copied then the move is applied and if move score is not null then recursive calculation are done to increase search depth
unsigned char recursiveSearchMove(GameData* gameData, unsigned char depth, unsigned char score) {
    unsigned char up, right, down, left;
    unsigned char grow = (((turn >> 1) + depth) % 10 == 0) ? 1 : 0;

    GameData* gdCopy1 = copyGameData(gameData);
    up = calcDir(gdCopy1, gameData, 0, grow, score);
    if(up && depth < MAX_DEPTH) up = recursiveSearchMove(gdCopy1, depth + 1, up) >> 2;
    freeMap(gdCopy1); free(gdCopy1);

    GameData* gdCopy2 = copyGameData(gameData);
    right = calcDir(gdCopy2, gameData, 1, grow, score);
    if(right && depth < MAX_DEPTH) right = recursiveSearchMove(gdCopy2, depth + 1, right) >> 2;
    freeMap(gdCopy2); free(gdCopy2);

    GameData* gdCopy3 = copyGameData(gameData);
    down = calcDir(gdCopy3, gameData, 2, grow, score);
    if(down && depth < MAX_DEPTH) down = recursiveSearchMove(gdCopy3, depth + 1, down) >> 2;
    freeMap(gdCopy3); free(gdCopy3);

    GameData* gdCopy4 = copyGameData(gameData);
    left = calcDir(gdCopy4, gameData, 3, grow, score);
    if(left && depth < MAX_DEPTH) left = recursiveSearchMove(gdCopy4, depth + 1, left) >> 2;
    freeMap(gdCopy4); free(gdCopy4);

    // Return better option, mope score is save on return[7 downto 2] and move direction on return[1 downto 0]
    if(up > right && up > down && up > left) return (up << 2) + 0;
    if(right > down && right > left) return (right << 2) + 1;
    if(down > left) return (down << 2) + 2;
    else return (left << 2) + 3;
}

// Wrapeer around recursive search move to only have move value in returned data
char calcMove(GameData* gameData, unsigned char maxDepth) {
    searchMap = allocGameMap(gameData->sizeX, gameData->sizeY);

    unsigned char res = recursiveSearchMove(gameData, 0, 0);
    printf("Calc move score: %d, and move: %d\n", res >> 2, res & 3);

    freeArray(searchMap, gameData->sizeX);
    return (unsigned char) res & 3;
}


int main() {
    connectToServer("localhost", 3456, "Valentin");

    // While loop for auto game restart
    while(1) {
        GameData* gameData = malloc(sizeof(GameData));
        initGame(gameData);
        
        while(1) {
            int code, move;
            char grow = ((turn >> 1) % 10 == 0) ? 1 : 0;

            if(!whoPlay) {
                char move = calcMove(gameData, 10);

                code = sendMove((t_move) move);
                moveSnake(gameData, 1, move, grow);
            } else {
                code = getMove((t_move *) &move);
                moveSnake(gameData, 0, move, grow);
            }

            if(!code) whoPlay = !whoPlay;
            else {
                if(!whoPlay) printf("\nYou loose \n");
                else printf("\nOpponent loose \n");
                break;
            }

            turn += 1;
        }

        turn = 0;
    }

    closeConnection();
    return 0;
}