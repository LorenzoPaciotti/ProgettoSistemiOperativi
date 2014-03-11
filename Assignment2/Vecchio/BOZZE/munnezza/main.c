#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <unistd.h>

int main(void){
	
	//messaggio benvenuto e chiedere numero macchine (volendo)
	int numeroMacchine=5;
	
	//~ initBenzinai(); 
	//aspetto su semaforo creazione benzinai
	
	//lancio init macchine, nel frattempo parte tutto
	//~ initFigliAuto(numeroMacchine);
	
	
	return 0;
	
}
