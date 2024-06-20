#include <stdio.h>
#include <stdlib.h>

#include "../../lib/snakeAPI/snakeAPI.h"

#include "cui.h"
#include "../structs/structs.h"

void printBoard(GameData* gameData) {
    // Move cursor up to height of the boards to rewrite on the lines
    // for(int i = 0; i < (gameData->sizeY + (gameData->sizeY - 1) + 2) * 2 + 1; i++) printf("\x1b[1A");

    // Print by API
    printArena();
    printf("\x1b[4A\x1b[2K");
    printf("\x1b[2A\x1b[2K");

    // New print fonction (better design)
    printMap(gameData);
}

// Better print map the server one, but totally unreadable
// Uses ANSI escaped codes to control cursor position
void printMap(GameData* gameData) {
    // Print top walls
    printf("┏━");
    for(unsigned char j = 0; j < gameData->sizeX; j ++) {
        printf("━");
        if(j + 1 < gameData->sizeX) {
            printf("━");

            if(gameData->map[j][0] & (1 << 1)) printf("┳");
            else printf("━");

            printf("━");
        }
    }
    printf("━┓\n");

    // Print map content
    for(unsigned char i = 0; i < gameData->sizeY; i ++) {
        // Print first line walls
        printf("┃ ");

        // Print vertical walls and snake body parts
        for(unsigned char j = 0; j < gameData->sizeX; j ++) {
            // Print snake or empty box
            if(gameData->map[j][i] & (7 << 4)) {
                // Calc snake color
                unsigned char r = gameData->map[j][i] & (1 << 7) ? 150 : 0;
                unsigned char g = gameData->map[j][i] & (1 << 7) ? 0 : 150;

                // Horizontal link to other snake body element
                if(j - 1 >= 0 && gameData->map[j - 1][i] & (7 << 4) && (((gameData->map[j - 1][i] >> 4) & 7) == 3 || ((gameData->map[j][i] >> 4) & 7) == 5)) {
                    printf("\x1b[2D\x1b[38;2;%d;%d;%dm■\x1b[0m\x1b[1C", r ? r - 30 : 0, g ? g -30 : 0, 0);
                }

                // Print head or body
                if((j == gameData->us.x && i == gameData->us.y) || (j == gameData->os.x && i == gameData->os.y)) printf("\x1b[38;2;%d;%d;%dm□\x1b[0m", r, g, 0);
                else printf("\x1b[38;2;%d;%d;%dm■\x1b[0m", r, g, 0);
            } else printf("\x1b[38;2;%d;%d;%dm▪\x1b[0m", 80, 80, 80);

            // Print right wall or space
            if(gameData->map[j][i] & (1 << 1)) printf(" ┃ ", 255, 255, 255);
            else printf("   ");
        }
        printf("\n");

        // if not last line
        if(i + 1 < gameData->sizeY) {
            // Print first line walls
            printf("┃");

            // Print horizontal walls
            for(unsigned char j = 0; j < gameData->sizeX; j ++) {
                // If there is a bottom wall
                if(gameData->map[j][i] & (1 << 2)) {
                    printf("━━━");

                    // Print corners and wall links
                    if(j - 1 >= 0 && i + 1 < gameData->sizeY && !(gameData->map[j - 1][i] & (1 << 2)) && gameData->map[j][i] & (1 << 3) && gameData->map[j][i + 1] & (1 << 3)) printf("\x1b[4D┣\x1b[3C");
                    else if(j - 1 >= 0 && i + 1 < gameData->sizeY && !(gameData->map[j - 1][i] & (1 << 2)) && gameData->map[j][i + 1] & (1 << 3)) printf("\x1b[4D┏\x1b[3C");
                    else if(j - 1 >= 0 && !(gameData->map[j - 1][i] & (1 << 2)) && gameData->map[j][i] & (1 << 3)) printf("\x1b[4D┗\x1b[3C");

                    if(j + 1 < gameData->sizeX && i + 1 < gameData->sizeY && gameData->map[j][i] & (1 << 1) && gameData->map[j][i + 1] & (1 << 1) && gameData->map[j + 1][i] & (1 << 2)) printf("╋");
                    else if(i + 1 < gameData->sizeY && gameData->map[j][i] & (1 << 1) && gameData->map[j][i + 1] & (1 << 1)) printf("┫");
                    else if(j + 1 < gameData->sizeX && gameData->map[j][i] & (1 << 1) && gameData->map[j + 1][i] & (1 << 2)) printf("┻");
                    else if(j + 1 < gameData->sizeX && i + 1 < gameData->sizeY && gameData->map[j][i + 1] & (1 << 1) && gameData->map[j + 1][i] & (1 << 2)) printf("┳");
                    else if(gameData->map[j][i] & (1 << 1)) printf("┛");
                    else if(i + 1 < gameData->sizeY && gameData->map[j][i + 1] & (1 << 1)) printf("┓");
                    else if(j + 1 < gameData->sizeX && gameData->map[j + 1][i] & (1 << 2)) printf("━");
                    else printf(" ");
                } else if(j - 1 >= 0 && i + 1 <= gameData->sizeY && !(gameData->map[j - 1][i] & (1 << 2)) && gameData->map[j][i] & (1 << 3) && gameData->map[j][i + 1] & (1 << 3)) {

                    // Link vertical walls
                    printf("   ");
                    printf("\x1b[4D┃\x1b[3C");

                    if(j + 1 < gameData->sizeX) printf(" ");
                } else {
                    // Link vertical snake tail
                    if(i + 1 < gameData->sizeY && gameData->map[j][i] & (7 << 4) && gameData->map[j][i + 1] & (7 << 4) && (((gameData->map[j][i] >> 4) & 7) == 4 || ((gameData->map[j][i + 1] >> 4) & 7) == 2)) {
                        unsigned char r = gameData->map[j][i] & (1 << 7) ? 120 : 0;
                        unsigned char g = gameData->map[j][i] & (1 << 7) ? 0 : 120;

                        printf("\x1b[38;2;%d;%d;%dm ■ \x1b[0m", r, g, 0);
                    } else printf("   ");

                    if(j + 1 < gameData->sizeX) printf(" ");
                }
            }

            // Print last line wall
            printf("┃\n");
        }
    }

    // Print bottom line
    printf("┗━");
    for(unsigned char j = 0; j < gameData->sizeX; j ++) {
        printf("━");
        if(j + 1 < gameData->sizeX) printf("━━━");
    }
    printf("━┛\n");
}