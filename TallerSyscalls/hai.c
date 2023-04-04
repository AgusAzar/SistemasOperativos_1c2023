#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int contador = 0;

void sigurgHandlerChild(int sig){
    printf("ya va!\n");
    contador ++;
}

void sigintHandlerParent(int sig){
    wait(NULL);
    exit(0);
}

int main(int argc, char* argv[]) {
    if(argc < 2 ) {
        printf("El programa espera al menos un argumento\n");
        return 1;
    }
    pid_t parentPid = getpid();
    pid_t childPid = fork();
    if(childPid == 0){
        signal(SIGURG, sigurgHandlerChild);
        while ( contador < 4 ) {
        }
        kill(parentPid, SIGINT);
        sleep(1);
        execvp(argv[1], argv + 1);
        exit(0);
    }
    else{
        signal(SIGINT, sigintHandlerParent);
        while(1){
            sleep(1);
            printf("sup!\n");
            kill(childPid, SIGURG);
        }
    }
}
