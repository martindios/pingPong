#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>

//Function declaration
static void signalHandler(int signal);

//Global variables
pid_t father;
FILE* tempfile;
typedef struct {
    pid_t pid;
    unsigned int pos;
    bool winner;
} Player;

Player player, machine;

int main (int argc, char **argv) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
    father = getpid();

    tempfile = tmpfile();

    if (tempfile == NULL) {
        perror("\e[91mError with temp file\e[0m\n");
        exit(1);
    }

    if(signal(SIGCONT, signalHandler) == SIG_ERR) {
        perror("\e[91mError defining the SIGCONT handler\e[0m\n");
        exit(1);
    }

    if(signal(SIGUSR1, signalHandler) == SIG_ERR) {
        perror("Error defining the SIGUSR1 handler\n");
        exit(1);
    }
 
    if(signal(SIGUSR2, signalHandler) == SIG_ERR) {
        perror("Error defining the SIGUSR2 handler\n");
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

    
    fclose(tempfile);

}

static void signalHandler(int signal) {
    printf("HOLA");
}
