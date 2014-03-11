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

typedef union _semun {
	int val;
	struct semid_ds *buf;
	ushort *array; } semun;

void creaSemafori(key_t semkey, int inizializzazione){
	int initsem(key_t semkey, int inizializzazione) {
	int status = 0, semid;
	semun arg;
	arg.val = inizializzazione;

	semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL);
	if (semid == -1) {
		if (errno == EEXIST) { 
				semid = semget(semkey, 1, 0);
					if (semid<0)
						perror("crea semaforo: ");
						return(-1);
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
	
}

struct my_msg {
	long mtype;
	int mtext;
}

void creaCodaMsg(key_t key){
	int msg_qid;
	msg_qid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
	if (msg_qid < 0){
		if (errno == EEXIST) {
			 msg_qid = msgget(key, 0);
			 if (msg_qid<0)
				perror("creaCoda: ");
				return(-1)
			 }
		else{
			printf(" errore: msg fallita RIGA 107 \n");
			exit(-1);
			}
	} 
	return msg_qid;
}


void creaMemCondivisa(key_t key){
	//~ key_t chiave = ftok("/home/lorenzo/hs_err_pid6420.log", 1); 
	int idmem = shmget(key,10*sizeof(int),IPC_CREAT | IPC_EXCL|0666);
	if(idmem < 0){
		if(errno==EEXIST){
			idmem = shmget(key,0,0);
			if(idmen<0)
				perror("memoria condivisa");
				return(-1);
			}
			}
	//~ perror("risultato shmget 144");
	printf("IDMEM SEM_MEM %d",idmem);
	return idmem;
}

void distruggiRisorse(){
	//aquisisco l'id dei semafori e li elimino
	printf("sto eliminando i semafori....... \n");
	
	//~ //SEMPERM 0600
	//~ int semPerm = semget(0600,1,0);
	//~ if (semPerm<0)
		//~ perror("aquisizione semaforo: ");
	//~ if ((semctl(semPerm,0,IPC_RMID,0))<0)
		//~ perror("rimozione semPerm: ");
	//~ 
	
	//SEM_STAZIONE 0601
	int semStazione = semget(0601,1,0);
	if (semStazione<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semStazione,0,IPC_RMID,0))<0)
		perror("rimozione semStazione: ");
	
	//SEM_POMPA1 0602
	int semPompa1 = semget(0602,1,0);
	if (semPompa1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPompa1,0,IPC_RMID,0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_POMPA2 0603
	int semPompa2 = semget(0603,1,0);
	if (semPompa2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPompa2,0,IPC_RMID, 0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_PIENOFATTO1 0604
	int semPienoFatto1 = semget(0604,1,0);
	if (semPienoFatto1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPienoFatto1,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto1: ");
	
	//SEM_PIENOFATTO2 0605
	int semPienoFatto2 = semget(0605,1,0);
	if (semPienoFatto2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semPienoFatto2,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto2: ");
	
	//SEM_SOLDIINVIATI1 0606
	int semSoldiInviati1 = semget(0606,1,0);
	if (semSoldiInviati1<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semSoldiInviati1,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati: ");
	
	//SEM_SOLDIINVIATI2 0607
	int semSoldiInviati2 = semget(0607,1,0);
	if (semSoldiInviati2<0)
		perror("aquisizione semaforo: ");
	if ((semctl(semSoldiInviati2,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati2: ");
	
	//SEM_VATTENE1 0610
	int vattene1 = semget(0610,1,0);
	if (vattene1<0)
		perror("aquisizione semaforo: ");	
	if ((semctl(vattene1,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene1: ");
	
	//SEM_VATTENE2 0611
	int vattene2 = semget(0611,1,0);
	if (vattene2<0)
		perror("aquisizione semaforo: ");
	if((semctl(vattene2,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene2: ");
	
	//SEM_MUTEXCASSA 0612
	int mutexCassa = semget(0612,1,0);
	if (mutexCassa<0)
		perror("aquisizione semaforo: ");
	if((semctl(mutexCassa,0,IPC_RMID, 0))<0)
		perror("rimozione mutexCassa: ");
	
	//SEM_POMPA1LIBERA 0613
	int pompa1Libera = semget(0613,1,0);
	if (pompa1Libera<0)
		perror("aquisizione semaforo: ");
	if((semctl(pompa1Libera,0,IPC_RMID, 0))<0)
		perror("rimozione sempompa1Libera: ");
	
	printf("fatto \n");
	
	//elimino la memoria condivisa
	printf("sto eliminando la memoria condivisa.......\n");
	//MEM_CASSA 0x700
	int memCassa = shmget(0700,1,0);
	if (memCassa<0)
		perror("aquisizione memoria condivisa: ");
	if((shmctl(memCassa,IPC_RMID,0))<0)
		perror("rimozione memCassa: ");
	
	printf("fatto \n");
	
	//elimino la coda dei messaggi
	printf("sto eliminando le code dei messaggi....... \n");
	//CODA_MESS1 0614
	int codaMess1 = msgget(0614,0);
	if (codaMess1<0)
		perror("aquisizione coda dei messaggi: ");
	if((msgctl(codaMess1,IPC_RMID,0))<0)
		perror("rimozione codaMess2: ");
	
	//CODA_MESS2 0615
	int codaMess2 = msgget(0615,0);
	if (codaMess2<0)
		perror("aquisizione coda dei messaggi: ");
	if((msgctl(codaMess2,IPC_RMID,0))<0)
		perror("rimozione codamess2: ");
	
	printf("fatto \n");
	
}


int main(){
	printf("PARTO");
	
	if((creaSemafori(SEM_STAZIONE,2))<0)
		printf("Semaforo SEM_STAZIONE");
	if((creaSemafori(SEM_POMPA1,0))<0)
		printf("Semaforo SEM_POMPA1");
	if((creaSemafori(SEM_POMPA2,0))<0)
		printf("Semaforo SEM_POMPA2");
	if((creaSemafori(SEM_PIENOFATTO1,0))<0)
		printf("Semaforo SEM_STAZIONE");
	if((creaSemafori(SEM_PIENOFATTO2,0))<0)
		printf("Semaforo SEM_PIENOFATTO2");
	if((creaSemafori(SEM_SOLDIINVIATI1,0))<0)
		printf("Semaforo SEM_SOLDIINVIATI1");
	if((creaSemafori(SEM_SOLDIINVIATI2,0))<0)
		printf("Semaforo SEM_SOLDIINVIATI2");
	if((creaSemafori(SEM_VATTENE1,0))<0)
		printf("Semaforo SEM_VATTENE1");
	if((creaSemafori(SEM_VATTENE2,0))<0)
		printf("Semaforo SEM_VATTENE2");
	if((creaSemafori(SEM_MUTEXCASSA,1))<0)
		printf("Semaforo SEM_MUTEXCASSA");
	if((creaSemafori(SEM_POMPA1LIBERA,1))<0)
		printf("Semaforo SEM_POMPA1LIBERA");
	if((creaSemafori(SEM_NUMEROMACCHINE,1))<0)
		printf("Semaforo SEM_NUMEROMACCHINE");
	if((creaCodaMsg(CODA_MESS1))<0)
		printf("errore CODAS_MESS1");
	if((creaCodaMsg(CODA_MESS2))<0)
		printf("errore CODA_MESS2");
	
	
	int idMem = creaMemCondivisa(MEM_CASSA)
		if (idMem<0)
			printf("errore crea memoria condivisa");
	printf("\nid memoria condivisa: %d \n",idMem);	
	int *punt1 = (int *) shmat(idMem, NULL, 0);	//puntatore alla memoria condivisa
	
	punt1[1] = 1; //inizializzo unofree
	punt1[2] = 5; //inizializzo numero macchine
	printf("\ninit unofree = %d \n",punt1[1]);
	printf("\ninit numero macchine = %d \n",punt1[2]);
	
	printf("CREATO TUTTO\n");
	
	
	pid_t benzinaio;
	benzinaio=fork();
	if (benzinaio<0){
		perror("fork benzinaio: ");
		distruggiRisorse();
		exit(-1);
	}
	if (benzinaio==0){
		if((execl( "./bin/benzinaioDIOPORCO", "benzinaioDIOPORCO" ,NULL,(char *) 0))<0)
			perror("exec benzinaio: ");
		wait(NULL);
		distruggiRisorse();
		exit(-1);	
	}
	printf("fatto \n");
	
	
	pid_t macchina;
	macchina=fork();
	if(macchina<0){
		perror("fork macchina: ");		
		distruggiRisorse();
		wait(NULL);
		exit(-1);
		}
	if (macchina==0){
		if ((execl("./bin/macchinaDIOPORCO","macchinaDIOPORCO",NULL,(char *) 0))<0)
			perror("exec macchina: ");
		wait(NULL);
		wait(NULL);
		distruggiRisorse();
		exit(-1);
	}
	printf("fatto \n");
	
	
	//faccio le wait ai processi macchina e benzinaio
	wait(NULL);
	wait(NULL);
	
	distruggiRisorse();
	
	printf("\n...TERMINAZIONE DEL PROGRAMMA... \n");
	
	
	return 0;
	}
