#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 10
#ifndef D
#define D 1
#endif

//Declaration of functions
static void signalHandler(int signal);
void drawBoard();

//Global variables
unsigned int board[SIZE][SIZE];
pid_t father;
FILE* tempfile;
typedef struct {
    pid_t pid;
    short pos; //Player's position
    short ball; //Position where the ball comes back (towards the player)
    unsigned short points;
    bool winner;
} Player;

Player player, machine;
int ret; //value of return children

int main (int argc, char **argv) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
    srand((int)time(NULL));

    player.pos = -1;
    player.points = 0;
    player.winner = false;
    machine.pos = -1;
    machine.points = 0;
    machine.winner = false;

    father = getpid();

    tempfile = tmpfile();

    if (tempfile == NULL) {
        perror("\e[91mError defining temp file\e[0m\n");
        exit(1);
    }

    struct sigaction sig;
    sig.sa_handler = signalHandler;
    sig.sa_flags = SA_RESTART;

    if(sigaction(SIGHUP, &sig, NULL)) {
        perror("\e[91mError defining SIGHUP handler\e[0m\n");
        exit(1);
    }

    if(sigaction(SIGUSR1, &sig, NULL)) {
        perror("Error defining SIGUSR1 handler\n");
        exit(1);
    }

    if(sigaction(SIGUSR2, &sig, NULL)) {
        perror("Error defining SIGUSR2 handler\n");
        exit(1);
    }

    
    if((player.pid = fork()) == -1) {
        perror("Error creating player process\n");
        exit(1);
    } else if (player.pid == 0){
        while(true);
    }

    if((machine.pid = fork()) == -1) {
        perror("Error creating machine process\n");
        exit(1);
    } else if (machine.pid == 0) {
        while(true);
    }

    //Father
    fwrite(&player, sizeof(Player), 1, tempfile);
    fwrite(&machine, sizeof(Player), 1, tempfile);
    rewind(tempfile);
    

    if(rand()%2==0){
        printf("Machine turn\n");
        kill(machine.pid, SIGUSR1);
    } else {
        printf("Player turn\n");
        kill(player.pid, SIGUSR1);
    }
    while(true);


    //fclose(tempfile); 

}

static void signalHandler(int signal) {
    pid_t pid = getpid(); 
    short int displ;

    fread(&player, sizeof(Player), 1, tempfile);
    fread(&machine, sizeof(Player), 1, tempfile);
    rewind(tempfile);

    switch(signal){ 
        case SIGUSR1: //Throw the ball
            if(pid == father){
                perror("Parent process shouldn't receive SIGUSR1\n");
                exit(1);
            } else if(pid == player.pid){ //Player
                do {
                    printf("Select position [0-9]: ");
                    scanf(" %hd", &player.pos);
                    if(player.pos > 9 || player.pos < 0) printf("Invalid position\n");
                } while(player.pos > 9 || player.pos < 0);

                printf("\n");
                
                printf("Player throws the ball from position %hd\n", player.pos);

                do{
                    printf("Select where you want to send the ball [0-9]: ");
                    scanf(" %hd", &machine.ball);
                    if(machine.ball > 9 || machine.ball < 0) printf("Invalid position\n");
                } while(machine.ball > 9 || machine.ball < 0);

                printf("\n");

                printf("Player throws the ball to the position %hd\n", machine.ball);

                fwrite(&player, sizeof(Player), 1, tempfile);
                fwrite(&machine, sizeof(Player), 1, tempfile);
                rewind(tempfile);

                kill(machine.pid, SIGUSR2);

            } else if(pid == machine.pid) {
                machine.pos = rand() % 10;
                player.ball = rand() % 10;

                printf("Machine throws the ball from position %hd\n", machine.pos);
                #if D==1
                printf("Machine throws the ball to the position %hd\n", player.ball); //DEPURATION ONLY
                #endif

                fwrite(&player, sizeof(Player), 1, tempfile);
                fwrite(&machine, sizeof(Player), 1, tempfile);
                rewind(tempfile);

                kill(player.pid, SIGUSR2);
            } else {
                perror("Unknown PID\n");
                exit(1);
            }

            break;
        case SIGUSR2: //Receive the ball
            if(pid == father){ 
                perror("Parent process shouldn't receive SIGUSR1\n");
                exit(1);
            } else if (pid == player.pid) {
                if(player.pos == -1){
                    do{
                        printf("Select position: ");
                        scanf(" %hd", &player.pos);
                        if(player.pos > 9 || player.pos < 0) printf("Invalid pos\n");
                    } while(player.pos > 9 || player.pos < 0);

                    printf("Player set in position %hd\n", player.pos);
                }

                do {
                    printf("Select displacement: [-2 2 desde %hd]", player.pos);
                    scanf(" %hd", &displ);
                    if(player.pos + displ > 9 || player.pos + displ < 0 || displ < -2 || displ > 2) printf("Invalid displacement\n");
                } while(player.pos + displ > 9 || player.pos + displ < 0 || displ < -2 || displ > 2);
                printf("\n");
                player.pos += displ;

                printf("Player displace to position %hd\n", player.pos);

                if(abs(player.pos - player.ball) > 3) { //Player lose the point
                    printf("Player couldn't return the ball\n");
                    machine.winner = true;
                    fwrite(&player, sizeof(Player), 1, tempfile);
                    fwrite(&machine, sizeof(Player), 1, tempfile);
                    rewind(tempfile);
                    sleep(1);
                    kill(father, SIGHUP);
                } else { //Player return the ball
                    printf("Player return the ball");
                    do{
                        printf("Select where to send the ball: ");
                        scanf(" %hd", &machine.ball);
                    } while(machine.ball > 9 || machine.ball < 0);
                    printf("\n");

                    printf("Player send the ball to position %hd", machine.ball);

                    fwrite(&player, sizeof(Player), 1, tempfile);
                    fwrite(&machine, sizeof(Player), 1, tempfile);
                    rewind(tempfile);

                    kill(machine.pid, SIGUSR2);
                }
            } else if(pid == machine.pid) {
                if(machine.pos == -1) { //initial position if not throw first
                    machine.pos = rand() % 10;
                    printf("Machine set in position %hd", machine.pos);
                }

                machine.pos += (rand() % 5) - 2; //{-2, ..., 2}
                if(machine.pos > 9) machine.pos = 9;
                else if (machine.pos < 0) machine.pos = 0;
                printf("Machine displace to position %hd\n", machine.pos);

                if(abs(machine.pos - machine.ball) > 3) { //Machine lose the point
                    printf("Machine couldn't return the ball\n");
                    player.winner = true;
                    fwrite(&player, sizeof(Player), 1, tempfile);
                    fwrite(&machine, sizeof(Player), 1, tempfile);
                    rewind(tempfile);

                    kill(father, SIGHUP);
                    
                } else { //Machine return the ball
                    printf("Machine return the ball\n");
                    player.ball = rand() % 10;
                    #if D == 1
                    printf("Machine send the ball to position %hd\n", player.ball);
                    #endif 

                    fwrite(&player, sizeof(Player), 1, tempfile);
                    fwrite(&machine, sizeof(Player), 1, tempfile);
                    rewind(tempfile);

                    kill(player.pid, SIGUSR2);
                }
            }

            break;

        case SIGHUP: //End (round/match)
            if(pid == father) {
                if(player.winner && !machine.winner) {
                    printf("Point to Player\n");
                    player.points++;
                } else if(!player.winner && machine.winner) {
                    printf("Point to Machine\n");
                    machine.points++;
                }

                if(player.points < 10 && machine.points < 10) { //End of round
                    printf("Player points: %hd\n", player.points);
                    printf("Machine points: %hd\n", machine.points);

                    player.winner = false;
                    machine.winner = false;

                    fwrite(&player, sizeof(Player), 1, tempfile);
                    fwrite(&machine, sizeof(Player), 1, tempfile);
                    rewind(tempfile);

                    if(rand() % 2 == 0) {
                        printf("Turn to Machine\n");
                        kill(machine.pid, SIGUSR1);
                    } else {
                        printf("Turn to Player\n");
                        kill(player.pid, SIGUSR1);
                    }
                } else { //End of match
                    if(player.points >= 10) printf("Player wins\n");
                    else if (machine.points >= 10) printf("Machine wins\n");
                    
                    kill(player.pid, SIGHUP);
                    waitpid(player.pid, &ret, 0);
                    printf("Player process finished\n");
                    kill(machine.pid, SIGHUP);
                    waitpid(machine.pid, &ret, 0);
                    printf("Machine process finished\n");
                    exit(0);
                }
            } else if(pid == player.pid) {
                exit(0);
            } else if(pid == machine.pid) {
                exit(0);
            }
            break;
    }
}

void drawBoard() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%hd", board[i][j]);
        }
        printf("\n");
    }
}
