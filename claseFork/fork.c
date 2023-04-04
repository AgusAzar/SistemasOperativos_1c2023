#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid == -1){
        //ocurrio un error
    }
    if(pid == 0){
        write(1,"Soy el hijo\n",12);
    }
    else{
        wait(NULL);
        write(1,"Soy el padre\n",13);
    }
}
