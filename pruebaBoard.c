#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define HEIGHT 20
#define WIDTH 80

unsigned int board[HEIGHT][WIDTH];
int blockPlayerX = WIDTH / 2;   // Posición inicial en el eje X
int blockWidth = 8;        // Ancho del bloque
int blockPlayerY = HEIGHT - 1; //Ubicacion en el eje Y

int blockMachineX = WIDTH / 2;
int blockMachineY = 1;

void fillBoard();
void drawBoard(int blockPlayerY);
void moveBlock(char direction, int *blockPlayerY);
void configureTerminal();
void resetTerminal();

int main() {
    configureTerminal();

    fillBoard();

    while (1) {
        drawBoard(blockPlayerY);
        char ch = getchar();
        if (ch == 'q') { // Presiona 'q' para salir
            system("clear");
            break;
        }
        moveBlock(ch, &blockPlayerY); // Pasa la dirección y la dirección del bloque
    }

    resetTerminal();
    return 0;
}

void fillBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0; // Llena el tablero con 0s
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

void moveBlock(char direction, int *blockPlayerY) {
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
