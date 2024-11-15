#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

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
