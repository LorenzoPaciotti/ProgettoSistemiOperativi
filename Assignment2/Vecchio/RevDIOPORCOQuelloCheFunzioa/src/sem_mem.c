#include "tutto.h"

int initsem(key_t semkey, int inizializzazione) {
	int status = 0, semid;
	semun arg;
	arg.val = inizializzazione;

	semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL);
	if (semid == -1) {
		if (errno == EEXIST) { 
				semid = semget(semkey, 1, 0);
				perror("43");
				//resetto i semafori se gia esistono
				semctl(semid, 0, SETVAL, arg);
				perror("Reset valori semafori");
				}
	} else {
		status = semctl(semid, 0, SETVAL, arg);
		perror("stato initSem 44");
	}
	if (semid == -1 || status == -1) {
		perror("initsem fallita RIGA 54");
		return(-1);
		}
	return(semid);
	
	}

int initMemoria(key_t key){
	//~ key_t chiave = ftok("/home/lorenzo/hs_err_pid6420.log", 1); 
	int idmem = shmget(key,10*sizeof(int),IPC_CREAT | IPC_EXCL|0666);
	if(idmem < 0){
		if(errno==EEXIST){
			idmem = shmget(key,0,0);
			perror("memoria già esistente");}
		else
			perror("err mem");}
	//~ perror("risultato shmget 144");
	printf("IDMEM SEM_MEM %d",idmem);
	return idmem;
}

int main(){
	
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
	printf("\n id memoria condivisa: %d \n",idMem);	
	int *punt1 = (int *) shmat(idMem, NULL, 0);	//puntatore alla memoria condivisa
	perror("attach 171");
	
	punt1[1] = 1; //inizializzo unofree
	punt1[2] = 5; //inizializzo numero macchine
	printf("\ninit unofree = %d \n",punt1[1]);
	printf("\ninit numero macchine = %d \n",punt1[2]);
	
	/* CODE MESSAGGI */
	crea_coda(CODA_MESS1);
	perror("risultato creazione coda 1");
	crea_coda(CODA_MESS2);
	perror("risultato creazione coda 2");
	
	return 0;
}
