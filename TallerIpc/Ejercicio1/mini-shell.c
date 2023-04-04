#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

static void runChildren(char ** prog, int pipes[][2], int n, int program_count){
	for(int i = 0; i < program_count - 1; ++i){
		if( i == n - 1){
			close(pipes[i][PIPE_WRITE]);
			dup2(pipes[i][PIPE_READ], STD_INPUT);
		}
		else if(i == n){
			close(pipes[i][PIPE_WRITE]);
			dup2(pipes[i][PIPE_READ], STD_OUTPUT);
		}
		else{
			close(pipes[i][PIPE_WRITE]);
			close(pipes[i][PIPE_READ]);
		}
	} 
	execvp(prog[0],prog);
}

static int run(char ***progs, size_t count)
{	
	int r, status;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);
	int pipes[count - 1][2];
	for(int i = 0; i < count - 1; ++i){
		int status = pipe(pipes[i]);
		if(status == -1 ) exit(-1);
	}
	
	for(int i = 0 ; i < count ; ++i){
		children[i] = fork();
		if(children[i] == 0){
			runChildren(progs[i], pipes, i, count);
		}
	} 

	for(int i = 0; i < count - 1; ++i){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	
	//TODO: Pensar cuantos procesos necesito
	//TODO: Pensar cuantos pipes necesito.

	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv)
{
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
