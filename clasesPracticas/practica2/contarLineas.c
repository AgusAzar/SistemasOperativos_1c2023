#include <time.h>
#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum pipeOperation { readPipe, writePipe };

int miPipe[2];

int procesoLs(){
    dup2(miPipe[writePipe], STDOUT_FILENO);
    char *args[] = {"ls", "-al", NULL};
    execvp("ls", args);
    exit(0);
}

int procesoWc(){
    dup2(miPipe[readPipe], STDIN_FILENO);
    close(miPipe[writePipe]);
    char *args[] = {"wc", "-l", NULL};
    execvp("wc", args);
    exit(0);
}

int main(int argc, char* argv[]) {
    if(pipe(miPipe) == -1){ exit(-1); }
    pid_t lsPId = fork();
    if(lsPId == -1 ) exit(-1);
    if(lsPId == 0){
        procesoLs();
    }
    pid_t wcPId = fork();
    if(wcPId == -1 ) exit(-1);
    if(wcPId == 0){
        procesoWc();
    }
    close(miPipe[readPipe]);
    close(miPipe[writePipe]);
}
