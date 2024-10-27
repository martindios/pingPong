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

Player P, M;

int main (int argc, char **argv) {
    setlocale(LC_CTYPE, "en_US.UTF-8");
    father = getpid();

    tempfile = tmpfile();

    printf("\e[91mError \e[0m\n");

    if(signal(SIGCONT, signalHandler) == SIG_ERR) {
        perror("Error defining the SIGCONT handler\n");
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

    /*
    if((P.pid = fork()) == -1) {
        perror("")
    }

    */
    fclose(tempfile);

}
