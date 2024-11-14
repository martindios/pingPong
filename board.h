#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

void fillBoard();
void drawBoard(int blockPlayerY);
void movePlayerBlock(char direction);
void moveMachineBlock();
void configureTerminal();
void resetTerminal();
void initBoard();


#endif
