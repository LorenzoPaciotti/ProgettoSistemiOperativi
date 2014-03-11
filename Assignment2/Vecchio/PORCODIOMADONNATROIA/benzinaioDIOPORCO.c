#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/shm.h>

int main(){
	
	pid_t pidRestituito; /* process id */
	pidRestituito = fork();
	
	if (pidRestituito < 0){
		printf("\n !!errore fork benzinaio, EXIT \n");
		exit(-1);
	}
		
	if (pidRestituito == 0)	//figlio (la stazione di servizio è a conduzione familiare)
		lavoroBenzinaioFiglio();
	else	//padre
		lavoroBenzinaioPadre();
		
	wait(NULL);
	exit(0);
}
