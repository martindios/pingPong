#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

#define HEIGHT 20
#define WIDTH 80

typedef struct {
    unsigned int board[HEIGHT][WIDTH];
    int blockPlayerX;
    int blockPlayerY;
    int blockMachineX;
    int blockMachineY;
    int blockWidth;
    int ballX;
    int ballY;
} GameStatus;

void fillBoard();
void drawBoard();
void movePlayerBlock(int pos);
void moveMachineBlock(int pos);
void configureTerminal();
void resetTerminal();
void initBoard();
void moveBallToMachine(int pos);
void moveBallToPlayer(int pos);


#endif
