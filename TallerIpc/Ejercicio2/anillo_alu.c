#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int generate_random_number(){
	return (rand() % 50);
}

static void cerrar_pipes(int pipes[][2], int n, int count){
	for(int i = -1; i < count; ++i){
		if( i == n - 1){
            int mod = (i + n) % n;
			close(pipes[mod][PIPE_WRITE]);
		}
		else if(i == n){
			close(pipes[i][PIPE_WRITE]);
		}
		else{
			close(pipes[i][PIPE_WRITE]);
			close(pipes[i][PIPE_READ]);
		}
	} 
}

static void ejecutar_start(int pipe_al_padre[2], int pipe_al_primer_hijo[2], int pipes[][2], int i, int count)
{
    cerrar_pipes(pipes, i, count);
    close(pipe_al_padre[PIPE_READ]);
    close(pipe_al_primer_hijo[PIPE_WRITE]);

    int pipe_a_leer = (i + count - 1) % count;
    int pipe_a_escribir = i;
    close(pipes[pipe_a_leer][PIPE_WRITE]);
    close(pipes[pipe_a_escribir][PIPE_READ]);
    int buffer;
    read(pipe_al_primer_hijo[PIPE_READ], &buffer, sizeof(buffer));
    int numeroRandom = generate_random_number();
    printf("El numero aleatorio es: %d\n", numeroRandom);
    do{
        buffer++;
        write(pipes[pipe_a_escribir][PIPE_WRITE],&buffer, sizeof(buffer));
        read(pipes[pipe_a_leer][PIPE_READ], &buffer, sizeof(buffer));
    }while(buffer < numeroRandom);

    write(pipe_al_padre[PIPE_WRITE],&buffer, sizeof(buffer));
    exit(0);
}

static void ejecutar_hijo( int pipes[][2], int i, int count){
    cerrar_pipes(pipes, i, count);
    int pipe_a_leer = (i + count - 1) % count;
    int pipe_a_escribir = i;
    close(pipes[pipe_a_leer][PIPE_WRITE]);
    close(pipes[pipe_a_escribir][PIPE_READ]);
    int buffer;
    while(read(pipes[pipe_a_leer][PIPE_READ], &buffer, sizeof(buffer))){
        buffer++;
        write(pipes[pipe_a_escribir][PIPE_WRITE],&buffer, sizeof(buffer));
    }
    exit(0);
}

int main(int argc, char **argv)
{	
	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);

    int pipes[n][2];
    int pipe_al_primer_hijo[2];
    int pipe_al_padre[2];
    if(pipe(pipe_al_padre) == -1 ) exit(-1);
    if(pipe(pipe_al_primer_hijo) == -1 ) exit(-1);
    for(int i = 0 ; i < n ; ++i ){
        if(pipe(pipes[i]) == -1 ) exit(-1);
    }

    for(int i = 0; i < n ; ++i){
        if(fork() == 0){
            if( i == start){
                ejecutar_start(pipe_al_padre,pipe_al_primer_hijo, pipes, i, n);
            }
            else{
                ejecutar_hijo(pipes, i, n);
            }
        }
    }
    cerrar_pipes(pipes, -2, n);
    close(pipe_al_padre[PIPE_WRITE]);
    close(pipe_al_primer_hijo[PIPE_READ]);
    write(pipe_al_primer_hijo[PIPE_WRITE],&buffer, sizeof(buffer));
    read(pipe_al_padre[PIPE_READ],&buffer, sizeof(int));
    printf("%d\n",buffer);
}
