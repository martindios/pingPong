#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "board.h"

#define HEIGHT 20
#define WIDTH 80

unsigned int board[HEIGHT][WIDTH];
int blockPlayerX = WIDTH / 2; // Initial position on the X-axis
int blockWidth = 8; // Block width
int blockPlayerY = HEIGHT - 1; // Position on the Y-axis

int blockMachineX = WIDTH / 2;
int blockMachineY = 1;

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
        movePlayerBlock(ch, &blockPlayerY);
        moveMachineBlock(ch, &blockMachineY);
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
    // Dibuja la parte superior de la caja
    printf("#");
    for (int j = 0; j < WIDTH; j++) {
        printf("#"); // Dibuja el borde superior
    }
    printf("#\n");

    for (int i = 0; i < HEIGHT; i++) {
        printf("#"); // Dibuja el borde izquierdo
        for (int j = 0; j < WIDTH; j++) {
            // Imprime el bloque basado en su posición y dimensiones
            if (i == blockPlayerY && j >= blockPlayerX && j < blockPlayerX + blockWidth) {
                printf("O"); // Imprime el bloque Player
            } else if(i == blockMachineY && j >= blockMachineX && j < blockMachineX + blockWidth) {
                printf("O");
            } else {
                printf(" "); // Espacio vacío
            }
        }
        printf("#\n"); // Dibuja el borde derecho
    }

    // Dibuja la parte inferior de la caja
    printf("#");
    for (int j = 0; j < WIDTH; j++) {
        printf("#"); // Dibuja el borde inferior
    }
    printf("#\n");
}

void movePlayerBlock(char direction, int *blockPlayerY) {
    // Limpia la posición actual del bloque
    for (int j = *blockPlayerY; j < *blockPlayerY + blockWidth; j++) {
        if (j < WIDTH) {
            board[*blockPlayerY][j] = 0; // Limpia el bloque
        }
    }

    switch (direction) {
        case 'a': // Izquierda
            //if (blockPlayerX > 0) blockPlayerX--;
            if (blockPlayerX > 0) blockPlayerX-=8;
            break;
        case 'd': // Derecha
            //if (blockPlayerX + blockWidth < WIDTH) blockPlayerX++;
            if (blockPlayerX + blockWidth < WIDTH) blockPlayerX +=8;
            break;
    }

    // Coloca el bloque en la nueva posición
    for (int j = blockPlayerX; j < blockPlayerX + blockWidth; j++) {
        if (*blockPlayerY < HEIGHT && j < WIDTH) {
            board[*blockPlayerY][j] = 1; // Coloca el bloque
        }
    }
}

void moveMachineBlock(char direction, int *blockMachineY) {
    // Limpia la posición actual del bloque
    for (int j = *blockMachineY; j < *blockMachineY + blockWidth; j++) {
        if (j < WIDTH) {
            board[*blockMachineY][j] = 0; // Limpia el bloque
        }
    }

    switch (direction) {
        case 'a': // Izquierda
            //if (blockPlayerX > 0) blockPlayerX--;
            if (blockMachineX > 0) blockMachineX-=8;
            break;
        case 'd': // Derecha
            //if (blockPlayerX + blockWidth < WIDTH) blockPlayerX++;
            if (blockMachineX + blockWidth < WIDTH) blockMachineX +=8;
            break;
    }

    // Coloca el bloque en la nueva posición
    for (int j = blockMachineX; j < blockMachineX + blockWidth; j++) {
        if (*blockMachineY < HEIGHT && j < WIDTH) {
            board[*blockMachineY][j] = 1; // Coloca el bloque
        }
    }
}

void configureTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag &= ~ICANON; // Desactiva el modo canónico
    new_settings.c_lflag &= ~ECHO;    // Desactiva el eco de la entrada
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void resetTerminal() {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &new_settings);
    new_settings.c_lflag |= ICANON; // Activa el modo canónico
    new_settings.c_lflag |= ECHO;    // Activa el eco de la entrada
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}
