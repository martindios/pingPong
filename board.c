#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "board.h"

#define HEIGHT 20
#define WIDTH 80

unsigned int board[HEIGHT][WIDTH];
int blockPlayerX = WIDTH / 2; // Initial position on the X-axis
int blockPlayerY = HEIGHT - 2; // Position on the Y-axis

int blockMachineX = WIDTH / 2;
int blockMachineY = 1;

int blockWidth = 8; // Block width

int ballX = WIDTH / 2;
int ballY = HEIGHT - 3;


void initBoard() {
    configureTerminal();

    fillBoard();

    /*
    while (1) {
        drawBoard();
        char ch = getchar();
        if (ch == 'q') { // Press 'q' to escape
            system("clear");
            break;
        }
        movePlayerBlock(ch);
        moveMachineBlock();
    } */

    //resetTerminal();
}

void fillBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0; 
        }
    }
}

void drawBoard() {
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
            if (i == ballY && j == ballX) {
                printf("o"); //Draw the ball
            } else if (i == blockPlayerY && j >= blockPlayerX && j < blockPlayerX + blockWidth) {
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

void movePlayerBlock(int pos) {
    // Clear the current position of the player block
    for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
        if (blockPlayerY < HEIGHT && j < WIDTH) {
            board[blockPlayerY][j] = 0; // Clear the block
        }
    }

    int targetX = 8 * pos;
    int step = (targetX > blockPlayerX) ? 1 : -1; // Move to the right or left
    
    while(blockPlayerX != targetX) {
        blockPlayerX += step;

        // Clear the current position of the player block
        for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
            if (blockPlayerY < HEIGHT && j < WIDTH) {
                board[blockPlayerY][j] = 0; // Clear the block
            }
        }

        for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
            if (blockPlayerY < HEIGHT && j < WIDTH) {
                board[blockPlayerY][j] = 1; // Place the block
            }
        }

        drawBoard();

        usleep(50000);
    }

}

void moveMachineBlock(int pos) {
    // Clear the current position of the machine block
    for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
        if (blockMachineY < HEIGHT && j < WIDTH) {
            board[blockMachineY][j] = 0; // Clear the block
        }
    }

    int targetX = 8 * pos;
    int step = (targetX > blockMachineX) ? 1 : -1; // Move to the right or left

    while(blockMachineX != targetX) {
        blockMachineX += step;

        // Clear the current position of the machine block
        for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
            if (blockMachineY < HEIGHT && j < WIDTH) {
                board[blockMachineY][j] = 0; // Clear the block
            }
        }

        for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
            if (blockMachineY < HEIGHT && j < WIDTH) {
                board[blockMachineY][j] = 1; // Place the block
            }
        }

        drawBoard();

        usleep(50000);
    }

    // Clear the final position of the machine block
    for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
        if (blockMachineY < HEIGHT && j < WIDTH) {
            board[blockMachineY][j] = 0; // Clear the block
        }
    }
}


void moveBallToMachine(int pos) { //Use Bresenham's line algorithm
    board[ballY][ballX] = 0; // Clear the ball
    
    int targetY = 2;
    int targetX = 8 * pos;

    int dx = targetX - ballX;
    int dy = targetY - ballY;
    int absDx = abs(dx);
    int absDy = abs(dy);
    int sx = (dx > 0) ? 1 : -1; //Step in the x-axis
    int sy = (dy > 0) ? 1 : -1; //Step in the y-axis
    int err = absDx - absDy;

    while (ballY != targetY || ballX != targetX) {
        board[ballY][ballX] = 0; // Clear the ball
        
        if (err * 2 > -absDy) {
            err -= absDy;
            ballX += sx;
        }
        if (err * 2 < absDx) {
            err += absDx;
            ballY += sy;
        }

        board[ballY][ballX] = 1; // Place the ball
        drawBoard();
        usleep(50000);
    }
}

void moveBallToPlayer(int pos) {
    board[ballY][ballX] = 0; // Clear the ball

    int targetY = HEIGHT - 3; // La posición objetivo en Y
    int targetX = 8 * pos; // La posición objetivo en X

    int dx = targetX - ballX;
    int dy = targetY - ballY;
    int absDx = abs(dx);
    int absDy = abs(dy);
    int sx = (dx > 0) ? 1 : -1; // Paso en el eje X
    int sy = (dy > 0) ? 1 : -1; // Paso en el eje Y
    int err = absDx - absDy;

    while (ballY != targetY || ballX != targetX) {
        board[ballY][ballX] = 0; // Clear the ball

        if (err * 2 > -absDy) {
            err -= absDy;
            ballX += sx; // Mover en X
        }
        if (err * 2 < absDx) {
            err += absDx;
            ballY += sy; // Mover en Y
        }

        board[ballY][ballX] = 1; // Place the ball
        drawBoard();
        usleep(1000000);
    }
}


void configureTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag &= ~ICANON; // Disable canonical mode
    //new_settings.c_lflag &= ~ECHO;    // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void resetTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag |= ICANON; // Enable canonical mode
    //new_settings.c_lflag |= ECHO;    // Enable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}
