#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int bart(){
    printf("Soy Bart\n");
    exit(0);
}
int lisa(){
    printf("Soy Lisa\n");
    exit(0);
}
int maggie(){
    printf("Soy Maggie\n");
    exit(0);
}
int process_a(){
    printf("Soy Abraham\n");
    if(fork() == 0){
        printf("Soy Homero\n");
        if(fork() == 0){
            bart();
        } else if(fork() == 0){
            lisa();
        } else if(fork() == 0){
            maggie();
        }
        exit(0);
    }
    return 0;
}

int process_b(){
    printf("Soy Abraham\n");
    int homero_pid = fork();
    if(homero_pid == 0){
        printf("Soy Homero\n");
        int bart_pid, lisa_pid, maggie_pid;
        bart_pid = fork();
        if( bart_pid == 0){
            bart();
        }
        lisa_pid = fork();
        if(lisa_pid == 0){
            lisa();
        } 
        maggie_pid = fork();
        if(maggie_pid == 0){
            maggie();
        }
        waitpid(bart_pid, NULL, 0);
        waitpid(lisa_pid, NULL, 0);
        waitpid(maggie_pid, NULL, 0);
        exit(0);
    }
    else{
        waitpid(homero_pid, NULL, 0);
    }
    return 0;
}


int main(){
    process_a();
    sleep(2);
    printf("--------------------------------\n");
    process_b();
}
