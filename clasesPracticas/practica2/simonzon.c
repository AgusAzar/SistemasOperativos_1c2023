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

int dado(){
    srand(time(0));
    int rnd = random();
    return rnd%6;
}

void hijo(int pipe[2]){
    int numero = dado();
    write(pipe[writePipe], &numero, sizeof(int));
    exit(0);
}

int main(int argc, char* argv[]) {
    int pipeLester[2];
    int pipeEliza[2];
    if(pipe(pipeLester) == -1 ) exit(-1);
    if(pipe(pipeEliza) == -1 ) exit(-1);
    pid_t lesterPid = fork();
    if(lesterPid == 0){
        hijo(pipeLester);
    }
    sleep(1); // para que la seed del random sea diferente
    pid_t elizaPid = fork();
    if(elizaPid == 0)
    {
        hijo(pipeEliza);
    }
    int valorLester;
    read(pipeLester[readPipe], &valorLester, sizeof(int));
    int valorEliza;
    read(pipeEliza[readPipe], &valorEliza, sizeof(int));
    if(valorLester > valorEliza){
        printf("Gano Lester\n");
    }
    else if(valorEliza > valorLester){
        printf("Gano Eliza\n");
    }
    else{
        printf("Empate\n");
    }
}
