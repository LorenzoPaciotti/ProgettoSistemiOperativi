#include <sys/msg.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SEMPERM 0600
#define SEM_STAZIONE 0601
#define SEM_POMPA1 0602
#define SEM_POMPA2 0603
#define SEM_PIENOFATTO1 0604
#define SEM_PIENOFATTO2 0605
#define SEM_SOLDIINVIATI1 0606
#define SEM_SOLDIINVIATI2 0607
#define SEM_VATTENE1 0610
#define SEM_VATTENE2 0611
#define SEM_MUTEXCASSA 0612
#define SEM_POMPA1LIBERA 0613
#define MEM_CASSA 0700
#define SEM_NUMEROMACCHINE 0616
#define	CODA_MESS1 0614
#define CODA_MESS2 0615

/* SEMAFORI */
typedef union _semun {
	int val;
	struct semid_ds *buf;
	ushort *array; } semun;

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
				//perror("Reset valori semafori");
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

int destroySem(int semid){
	
	//disallocazione semaforo semid
	semun arg;
	arg.val = 0;
	if(semctl(semid,0,IPC_RMID, arg)==0){
		perror("Distruzione Di Un Semaforo Fallita RIGA 67 \n");
		exit(2);
		}
	return 0;
}

/* CODE */
struct my_msg {
	long mtype;
	int mtext;
};

void scrivi_msg(int coda,struct my_msg *a,int importo){
	int rispostamsg;
		a->mtype=1;
		a->mtext=importo;
		if( (rispostamsg = msgsnd(coda,a,sizeof(int),0))==-1){
			perror("errore msgsnd RIGA 84 \n");
			exit(3);
			}
					
}

void ricevi_msg(int coda,struct my_msg *a){
	int mlen;
		if ( (mlen = msgrcv(coda, a,sizeof(int),0, MSG_NOERROR) == -1)){
			perror("errore nella msgrcv; RIGA 93\n");
			exit(4);}
		//printf("dato ricevuto %d\n",a->mtext);
		
}

int  crea_coda( key_t key ){
	int msg_qid;
	msg_qid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
	if (msg_qid < 0){
		if (errno == EEXIST) {
			 msg_qid = msgget(key, 0);
			 }
		else{
			printf(" errore: msg fallita RIGA 107 \n");
			exit(-1);
			}
	} 
	return msg_qid;
		
}

int distruggi_coda(int key){
	int Msgctl;
	if((Msgctl=msgctl(key,IPC_RMID,0))==-1){
		perror("Errore Distruzione Coda; RIGA 118 sem_mem \n");
		}
	return 0;
}


/* MEMORIA CONDIVISA */
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
	//printf("IDMEM SEM_MEM %d",idmem);
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
	//printf("\n id memoria condivisa: %d \n",idMem);	
	int *punt1 = (int *) shmat(idMem, NULL, 0);	//puntatore alla memoria condivisa
	//perror("attach 171");
	
	punt1[1] = 1; //inizializzo unofree
	punt1[2] = 2; //inizializzo numero macchine
	punt1[3] = 0;
//printf("\ninit unofree = %d \n",punt1[1]);
	printf("\ninit numero macchine = %d \n",punt1[2]);
	
	/* CODE MESSAGGI */
	crea_coda(0614);
	//perror("risultato creazione coda 1");
	crea_coda(0615);
	//perror("risultato creazione coda 2");
	
	return 0;
	}
	


/*Identify the semaphores: ipcs 
ipcs -q List share queues. 
ipcs -m Shared memory. 
ipcs -s List Semaphores.
Remove the semaphores: ipcrm -s <ipcs id>*/



//sem stazione=2;
//sem pompa1, pompa2 = 0;
//sem pieno_fatto1=0;
//sem pieno_fatto2=0;
//sem soldi_inviati1=0;
//sem soldi_inviati2=0;
//sem vattene1=0;
//sem vattene2=0;
//sem mutex_cassa=1;
//sem mutex_pompa1libera;

//shmem int cassa
//msgq coda_messaggi_pompa1
//msgq coda_messaggi_pompa2

//shmem int unofree=1;
