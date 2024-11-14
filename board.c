#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "board.h"

#define HEIGHT 20
#define WIDTH 80

unsigned int board[HEIGHT][WIDTH];
int blockPlayerX = WIDTH / 2; // Initial position on the X-axis
int blockPlayerY = HEIGHT - 1; // Position on the Y-axis

int blockMachineX = WIDTH / 2;
int blockMachineY = 1;

int blockWidth = 8; // Block width


void initBoard() {
    configureTerminal();

    fillBoard();

    while (1) {
        drawBoard(blockPlayerY);
        char ch = getchar();
        if (ch == 'q') { // Press 'q' to escape
            system("clear");
            break;
        }
        movePlayerBlock(ch);
        moveMachineBlock();
    }

    resetTerminal();
}

void fillBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0; 
        }
    }
}

void drawBoard(int blockPlayerY) {
    system("clear");
    // Draw the top border
    printf("#");
    for (int j = 0; j < WIDTH; j++) {
        printf("#"); 
    }
    printf("#\n");

    for (int i = 0; i < HEIGHT; i++) {
        printf("#"); // Draw the left border
        for (int j = 0; j < WIDTH; j++) { 
            if (i == blockPlayerY && j >= blockPlayerX && j < blockPlayerX + blockWidth) {
                printf("O"); //Draw the player block 
            } else if(i == blockMachineY && j >= blockMachineX && j < blockMachineX + blockWidth) {
                printf("O"); //Draw the machine block
            } else {
                printf(" "); // Empty space
            }
        }
        printf("#\n"); // Draw the right border
    }

    // Draw the bottom border
    printf("#");
    for (int j = 0; j < WIDTH; j++) {
        printf("#");
    }
    printf("#\n");
}

void movePlayerBlock(char direction) {
    // Clear the current position of the player block
    for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
        if (blockPlayerY < HEIGHT && j < WIDTH) {
            board[blockPlayerY][j] = 0; // Clear the block
        }
    }

    switch (direction) {
        case 'a': // Left
            if (blockPlayerX > 0) blockPlayerX-=8;
            break;
        case 'd': // Right
            if (blockPlayerX + blockWidth < WIDTH) blockPlayerX +=8;
            break;
    }

    // Place the player block in the new position
    for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
        if (blockPlayerY < HEIGHT && j < WIDTH) {
            board[blockPlayerY][j] = 1; // Place the block
        }
    }
}

void moveMachineBlock() {
    // Clear the current position of the machine block
    for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
        if (blockPlayerX < HEIGHT && j < WIDTH) {
            board[blockMachineY][j] = 0; // Clear the block
        }
    }

    if (blockMachineX + blockWidth < WIDTH) {
        blockMachineX += 8;
    } else {
        blockMachineX = 0;
    }

    for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
        if (blockMachineY < HEIGHT && j < WIDTH) {
            board[blockMachineY][j] = 1; // Place the block
        }
    }
}

void configureTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag &= ~ICANON; // Disable canonical mode
    new_settings.c_lflag &= ~ECHO;    // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void resetTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag |= ICANON; // Enable canonical mode
    new_settings.c_lflag |= ECHO;    // Enable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}
