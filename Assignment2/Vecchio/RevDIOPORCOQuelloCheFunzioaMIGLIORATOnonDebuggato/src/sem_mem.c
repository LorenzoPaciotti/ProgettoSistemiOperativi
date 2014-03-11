#include "tutto.h"

int initsem(key_t semkey, int inizializzazione);
int initMemoria(key_t key);

int main(int numeroMacchine){	
	/* SEMAFORI */
	//sem stazione=2;
	initsem(SEM_STAZIONE,2);
	//sem pompa1
	initsem(SEM_POMPA1,0);
	//sem pompa2
	initsem(SEM_POMPA2,0);
	//sem pieno_fatto1=0;
	initsem(SEM_PIENOFATTO1,0);
	//sem pieno_fatto2=0;
	initsem(SEM_PIENOFATTO2,0);
	//sem soldi_inviati1=0;
	initsem(SEM_SOLDIINVIATI1,0);
	//sem soldi_inviati2=0;
	initsem(SEM_SOLDIINVIATI2,0);
	//sem vattene1=0;
	initsem(SEM_VATTENE1,0);
	//sem vattene2=0;
	initsem(SEM_VATTENE2,0);
	//sem mutex_cassa=1;
	initsem(SEM_MUTEXCASSA,1);
	//sem mutex_pompa1libera;
	initsem(SEM_POMPA1LIBERA,1);
	//sem variabile numero macchine
	initsem(SEM_NUMEROMACCHINE,1);
	
	/* MEMORIA CONDIVISA in posizione[0] CASSA, in posizione [1] UNOFREE, in posizione [2] NUMEROMACCHINE */ 
	int idMem = initMemoria(MEM_CASSA);	
	int *punt1 = (int *) shmat(idMem, NULL, 0);	//puntatore alla memoria condivisa
	if (punt1<0)
		perror("shmat memoria condivisa: ");
	
	punt1[1] = 1; //inizializzo la variabile Pompa uno libera a true
	punt1[2] = numeroMacchine; //inizializzo la variabile numero macchine
	punt1[3] = 0; //inizializzo la variabile fine giornata a false
	printf("numero macchine = %d \n",punt1[2]);
	
	/* CODE MESSAGGI */
	crea_coda(CODA_MESS1);
	crea_coda(CODA_MESS2);
	
	return 0;
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
	if (semid == -1 || status == -1) {
		printf("initsem fallita");
		return(-1);
		}
	return(semid);
	}

int initMemoria(key_t key){
	int idmem = shmget(key,10*sizeof(int),IPC_CREAT | IPC_EXCL|0666);
	if(idmem < 0){
		if(errno==EEXIST){
			idmem = shmget(key,0,0);
			if (idmen<0)
				perror("shmget memoria condivisa");
				}
		else
			perror("err mem");}
	//printf("IDMEM SEM_MEM %d",idmem);
	return idmem;
}
