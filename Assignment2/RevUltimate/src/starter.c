#include "tutto.h"

void asciiTitolo();
void asciiSem_mem();
void asciiBenzinaio();
void asciiCredits();
void asciiMacchina();
void distruggiRisorse();

int main(){
	int *ritornoSemMem=NULL;
	char invio;
	
	asciiTitolo();
	
	printf("\nPremi un tasto per continuare\n");
	scanf("%c",&invio);
	printf ("\nStart del programma \n ");
	
	//creo il processo sem_mem
	asciiSem_mem();
	printf("creo il processo sem_mem......");
	pid_t sem_mem;
	sem_mem=fork();
	if (sem_mem<0){
		perror("fork sem_mem: ");
		exit(-1);
		}
	if (sem_mem==0){
		if (execl("./bin/sem_mem", "sem_mem", NULL, (char *) 0)<0){
			perror("exec sem_mem: ");
			exit(-1);
			}
		}
	
	wait(ritornoSemMem);
	if(ritornoSemMem<0){
		printf("errore nell'esecuzione di sem_men");
		distruggiRisorse();
		exit(-1);
		}
	printf("fatto\n");
	
	//creo il processo benzinaio
	asciiBenzinaio();
	 
	pid_t benzinaio;
	benzinaio=fork();
	if (benzinaio<0){
		perror("fork benzinaio: ");
		distruggiRisorse();
		exit(-1);
	}
	if (benzinaio==0){ //lo esegue il figlio
		if(execl( "./bin/benzinaio", "benzinaio" ,NULL,(char *) 0)<0)
			perror("exec benzinaio: ");
			distruggiRisorse();
			exit(-1);
	}
	printf("fatto\n");
	
	//creo il processo macchina
	asciiMacchina();
	
	pid_t macchina;
	macchina=fork();
	if(macchina<0){
		perror("fork macchina: ");		
		distruggiRisorse();
		wait(NULL);
		exit(-1);
		}
	if (macchina==0){
		if ((execl("./bin/macchina","macchina",NULL,(char *) 0))<0){
			perror("exec macchina: ");
			distruggiRisorse();
			exit(-1);
			}
	}
	printf("fatto \n");
	
	int *valoreRitorno=NULL;
	int boolErrore=0;
	/*faccio le wait ai processi macchina e benzinaio, gestendo i valori 
	di ritorno*/
	int i;
	for(i=0;i<2;i++){
		wait(valoreRitorno);
		if (valoreRitorno<0){
			boolErrore=1;
			printf ("errore nell'eseguzione del processo auto o benzinaio");
			}
	}

	distruggiRisorse();
	
	printf("\n...TERMINAZIONE DEL PROGRAMMA... \n");
	
	asciiCredits();
    
    if(boolErrore)
		exit(-1);		
	else
		exit(0);
}

void distruggiRisorse(){
	//aquisisco l'id dei semafori e li elimino
	printf("sto eliminando i semafori....... ");
	
	//SEM_STAZIONE 0601
	int semStazione = semget(SEM_STAZIONE,1,0);
	if (semStazione<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semStazione,0,IPC_RMID,0))<0)
		perror("rimozione semStazione: ");
	
	//SEM_POMPA1 0602
	int semPompa1 = semget(SEM_POMPA1,1,0);
	if (semPompa1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPompa1,0,IPC_RMID,0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_POMPA2 0603
	int semPompa2 = semget(SEM_POMPA2,1,0);
	if (semPompa2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPompa2,0,IPC_RMID, 0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_PIENOFATTO1 0604
	int semPienoFatto1 = semget(SEM_PIENOFATTO1,1,0);
	if (semPienoFatto1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPienoFatto1,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto1: ");
	
	//SEM_PIENOFATTO2 0605
	int semPienoFatto2 = semget(SEM_PIENOFATTO2,1,0);
	if (semPienoFatto2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPienoFatto2,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto2: ");
	
	//SEM_SOLDIINVIATI1 0606
	int semSoldiInviati1 = semget(SEM_SOLDIINVIATI1,1,0);
	if (semSoldiInviati1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semSoldiInviati1,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati: ");
	
	//SEM_SOLDIINVIATI2 0607
	int semSoldiInviati2 = semget(SEM_SOLDIINVIATI2,1,0);
	if (semSoldiInviati2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semSoldiInviati2,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati2: ");
	
	//SEM_VATTENE1 0610
	int vattene1 = semget(SEM_VATTENE1,1,0);
	if (vattene1<0)
		perror("aquisizione semaforo: ");	
	if ((semctl(vattene1,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene1: ");
	
	//SEM_VATTENE2 0611
	int vattene2 = semget(SEM_VATTENE2,1,0);
	if (vattene2<0)
		perror("aquisizione semaforo: ");
	if((semctl(vattene2,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene2: ");
	
	//SEM_MUTEXCASSA 0612
	int mutexCassa = semget(SEM_MUTEXCASSA,1,0);
	if (mutexCassa<0)
		perror("aquisizione semaforo: ");
	if((semctl(mutexCassa,0,IPC_RMID, 0))<0)
		perror("rimozione mutexCassa: ");
	
	//SEM_POMPA1LIBERA 0613
	int pompa1Libera = semget(SEM_POMPA1LIBERA,1,0);
	if (pompa1Libera<0)
		perror("aquisizione semaforo: ");
	if((semctl(pompa1Libera,0,IPC_RMID, 0))<0)
		perror("rimozione sempompa1Libera: ");
	
	//SEM_NUMEROMACCHINE 0616
	int numeroMacchine = semget(SEM_NUMEROMACCHINE,1,0);
	if (numeroMacchine<0)
		perror("aquisizione semaforo: ");
	if((semctl(numeroMacchine,0,IPC_RMID, 0))<0)
		perror("rimozione semaforo numeroMacchine: ");
	
	printf("fatto \n");
	
	//elimino la memoria condivisa
	printf("sto eliminando la memoria condivisa.......");
	//MEM_CONDIVISA 0x700
	int memCassa = shmget(MEM_CONDIVISA,1,0);
	if (memCassa<0)
		perror("aquisizione memoria condivisa: ");
	if((shmctl(memCassa,IPC_RMID,0))<0)
		perror("rimozione memCassa: ");
	
	printf("fatto \n");
	
	//elimino la coda dei messaggi
	printf("sto eliminando le code dei messaggi....... ");
	//CODA_MESS1 0614
	int codaMess1 = msgget(CODA_MESS1,0);
	if (codaMess1<0)
		perror("aquisizione coda dei messaggi: ");
	if((msgctl(codaMess1,IPC_RMID,0))<0)
		perror("rimozione codaMess2: ");
	
	//CODA_MESS2 0615
	int codaMess2 = msgget(CODA_MESS2,0);
	if (codaMess2<0)
		perror("aquisizione coda dei messaggi: ");
	if((msgctl(codaMess2,IPC_RMID,0))<0)
		perror("rimozione codamess2: ");
	
	printf("fatto \n");
	
}
