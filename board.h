#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

void fillBoard();
void drawBoard(int blockPlayerY);
void movePlayerBlock(char direction, int *blockPlayerY);
void moveMachineBlock(char direction, int *blockMachineY);
void configureTerminal();
void resetTerminal();
void initBoard();


#endif
