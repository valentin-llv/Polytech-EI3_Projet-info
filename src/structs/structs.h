#ifndef STRUCTS_H
#define STRUCTS_H

// Struct for one 2D coord
typedef struct Coord {
    unsigned char x;
    unsigned char y;
} Coord;

// Struct containing one game state, will be copied multiple times to simulate game
typedef struct GameData {
    unsigned char sizeX;
    unsigned char sizeY;

    Coord us; // User snake
    Coord ust; // TODO : Not sure if neccesary, is use snake tail

    Coord os; // Opponent snake

    unsigned char** map; // See bit map
} GameData;

// Bit map
// B0 wall up, B1 wall right, B2 wall down, B3 wall left
// B4, B5, B6: 0 = empty, 1 = full no neighbour, 2 = full up, 3 = full right, 4 = full down, 5 = full left
// (full mean square has snkae body in it, direction mean next body direction)

GameData* copyGameData(GameData* source);

unsigned char** allocGameMap(unsigned char sizeX, unsigned char sizeY);
void freeMap(GameData* gameData);
void freeArray(unsigned char** array, char sizeX);

#endif