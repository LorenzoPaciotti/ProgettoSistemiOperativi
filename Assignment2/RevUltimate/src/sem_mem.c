#include "tutto.h"

int initsem(key_t semkey, int inizializzazione);
int initMemoria(key_t key);

int main(){	
	/* SEMAFORI */
	//sem stazione=2;
	if (initsem(SEM_STAZIONE,2)<0){
		printf("Creazione semaforo SEM_STAZIONE: Fallita\n");
		exit(-1);
	}
	//sem pompa1
	if (initsem(SEM_POMPA1,0)<0){
		printf("Creazione semaforo SEM_POMPA1: Fallita\n");
		exit(-1);
	}
	//sem pompa2
	if (initsem(SEM_POMPA2,0)<0){
		printf("Creazione semaforo SEM_POMPA2: Fallita\n");
		exit(-1);
	}
	//sem pieno_fatto1=0;
	if (initsem(SEM_PIENOFATTO1,0)<0){
		printf("Creazione semaforo SEM_PIENOFATTO1: Fallita\n");
		exit(-1);
	}
	//sem pieno_fatto2=0;
	if (initsem(SEM_PIENOFATTO2,0)<0){
		printf("Creazione semaforo SEM_PIENOFATTO2: Fallita\n");
		exit(-1);
	}
	//sem soldi_inviati1=0;
	if (initsem(SEM_SOLDIINVIATI1,0)<0){
		printf("Creazione semaforo SEM_SOLDIINVIATI1: Fallita\n");
		exit(-1);
	}
	//sem soldi_inviati2=0;
	if (initsem(SEM_SOLDIINVIATI2,0)<0){
		printf("Creazione semaforo SEM_SOLDIINVIATI2: Fallita\n");
		exit(-1);
	}
	//sem vattene1=0;
	if (initsem(SEM_VATTENE1,0)<0){
		printf("Creazione semaforo SEM_VATTENE1: Fallita\n");
		exit(-1);
	}
	//sem vattene2=0;
	if (initsem(SEM_VATTENE2,0)<0){
		printf("Creazione semaforo SEM_VATTENE2: Fallita\n");
		exit(-1);
	}
	//sem mutex_cassa=1;
	if (initsem(SEM_MUTEXCASSA,1)<0){
		printf("Creazione semaforo SEM_MUTEXCASSA: Fallita\n");
		exit(-1);
	}
	//sem mutex_pompa1libera;
	if (initsem(SEM_POMPA1LIBERA,1)<0){
		printf("Creazione semaforo SEM_POMPA1LIBERA: Fallita\n");
		exit(-1);
	}
	//sem variabile numero macchine
	if (initsem(SEM_NUMEROMACCHINE,1)<0){
		printf("Creazione semaforo SEM_NUMEROMACCHINE: Fallita\n");
		exit(-1);
	}
	
	/* MEMORIA CONDIVISA in posizione[0] CASSA, in posizione [1] UNOFREE, in posizione [2] NUMEROMACCHINE */ 
	int idMem = initMemoria(MEM_CONDIVISA);	
	if (idMem<0){
		printf("creazione memoria condivisa: Fallita");
		exit(-1);
	}
	int *puntMem = (int *) shmat(idMem, NULL, 0);	//puntatore alla memoria condivisa
	if (puntMem<0){
		perror("shmat memoria condivisa: ");
		exit(-1);
	}
	
	puntMem[1] = 1; //inizializzo la variabile Pompa uno libera a true
	puntMem[2] = NUMERO_MACCHINE;	//inserisco in memoria condivisa il numero delle macchine
	puntMem[3] = 0; //inizializzo la variabile fine giornata a false
	printf("numero macchine = %d \n",puntMem[2]);
	
	/* CODE MESSAGGI */
	if(crea_coda(CODA_MESS1)<0){
		printf("creazione coda messaggi1: Fallita");
		exit(-1);
	}
		
	if (crea_coda(CODA_MESS2)<0){
		printf("creazione coda messaggi2: Fallita");
		exit(-1);
	}
	
	exit(0);
}

int initsem(key_t semkey, int inizializzazione) {
	int status = 0, semid;
	semun arg;
	arg.val = inizializzazione;
	semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL);
	if (semid<0) {
		if (errno == EEXIST) { 
				semid = semget(semkey, 1, 0);
				if (semid<0)
					perror("semget semaforo: ");
				//resetto i semafori se gia esistono
				if((semctl(semid, 0, SETVAL, arg))<0)
					perror("Reset valori semafori: ");
				}
	} else {
		status = semctl(semid, 0, SETVAL, arg);
		if (status<0)
			perror("semctl semaforo: ");
	}
	if (semid <0 || status <0) {
		printf("initSem fallita");
		return(-1);
		}
	return(semid);
	}

int initMemoria(key_t key){
	int idmem = shmget(key,10*sizeof(int),IPC_CREAT | IPC_EXCL|0666);
	if(idmem < 0){
		if(errno==EEXIST){
			idmem = shmget(key,0,0);
			if (idmem<0)
				perror("shmget memoria condivisa");
		} else {
			printf("initMemoria fallita");
			return(-1);
		}
	}
	return idmem;
}
