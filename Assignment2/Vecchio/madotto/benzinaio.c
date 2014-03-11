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
/* SEMAFORI E CODE */
struct my_msg {
	long mtype;
	int mtext;
};

typedef union _semun {
	int val;
	struct semid_ds *buf;
	ushort *array; } semun;

int waitSem(int semid) {
	struct sembuf wait_buf;
	wait_buf.sem_num = 0;
	wait_buf.sem_op = -1;
	wait_buf.sem_flg = 0;
	if(semop(semid, &wait_buf, 1) == -1) {
		perror("waitSem fallita");
		exit(1);
	}
	return(0);
}

int signalSem(int semid) {
	struct sembuf signal_buf;
	signal_buf.sem_num = 0;
	signal_buf.sem_op = 1;
	signal_buf.sem_flg = 0;
	if(semop(semid, &signal_buf, 1) == -1) {
		perror("signalSem fallita");
		exit(1);
	}
	return(0);
}

int destroySem(int semid){
	
	//disallocazione semaforo semid
	semun arg;
	arg.val = 0;
	semctl(semid,0,IPC_RMID, arg);
	
	return 0;
}

int  crea_coda( key_t key ){
	int msg_qid;
	msg_qid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
	if (msg_qid < 0){
		if (errno == EEXIST) {
			 msg_qid = msgget(key, 0);
			 }
		else{
			printf(" errore: msg fallita RIGA 127 \n");
			exit(-1);
			}
	} 
	return msg_qid;
		
}

void scrivi_msg(int coda,struct my_msg *a,int importo){
	int rispostamsg;
		a->mtype=1;
		a->mtext=importo;
		if( (rispostamsg = msgsnd(coda,a,sizeof(int),0))==-1){
			perror("errore msgsnd RIGA 102 \n");
			exit(3);
			}
					
}

void ricevi_msg(int coda,struct my_msg *a){
	int mlen;
		if ( (mlen = msgrcv(coda, a,sizeof(int),0, MSG_NOERROR) == -1)){
			perror("errore nella msgrcv; RIGA 113\n");
			exit(4);}
		//printf("dato ricevuto %d  da pid %d \n",a->mtext,getpid());
		
}

/*CODICE BENZINAIO*/

void lavoroBenzinaioPadre(){
	printf("\n lavoro padre benzinaio inizia - 93 \n");
	int coda1 = crea_coda(0614);
	int *puntMem;
	int shmid;
	shmid = shmget(0700, 10*sizeof(int),0666 );
	puntMem=(int *) shmat(shmid, NULL, 0666);
	int idSemPompa1 = semget(0602, 1, 0);
	int idSemPienoFatto1 = semget(0604,1,0);
	int idSemSoldiInviati1 = semget(0606,1,0);
	int idSemVattene1 = semget(0610,1,0);
	int idSemMutexCassa = semget(0612,1,0);
	int idNumMa = semget(0616,1,0);
	int idSemPompa2 = semget(0603, 1, 0);
	
	int numeroMacchineLocale;
	
	while(1){
		printf("\n WHILE padre benzinaio \n");
		waitSem(idNumMa);
			if(puntMem[2] == 0 )
				signalSem(idSemPompa1);
		signalSem(idNumMa);
		waitSem(idSemPompa1);
			waitSem(idNumMa);	
				printf("\n valore prima if padre %d \n", puntMem[2]);
				if(puntMem[2] == 0 ){
					printf("\n entrato if padre\n");
					signalSem(idNumMa);
					break;}
				printf("\n numero macchine rimaste prima del controllo padre %d \n", puntMem[2]);
				signalSem(idNumMa);
				struct my_msg *m=malloc(sizeof(struct my_msg));
				int importoDaChiedere = rand() % 100;
				scrivi_msg(coda1,m,importoDaChiedere);
					signalSem(idSemPienoFatto1); 
					waitSem(idSemSoldiInviati1); 
						ricevi_msg(coda1,m);
						int soldiPresi = m->mtext;
					
					signalSem(idSemVattene1);
				
					printf("\n fatta la vattene 1 \n"); 
					waitSem(idSemMutexCassa); 
						puntMem[0] = puntMem[0]+soldiPresi;
					signalSem(idSemMutexCassa);
				waitSem(idNumMa); 
					puntMem[2]=puntMem[2]-1;
					printf("\n numero macchine rimaste  padre: %d \n", puntMem[2]);
					if(puntMem[2] ==0) {
						signalSem(idSemPompa2);
						signalSem(idSemPompa1);
						signalSem(idNumMa);
						break;
						}
				signalSem(idNumMa);
			
	}

	printf("\npadre uscito dal while\n");
	
	signalSem(idSemPompa2);
	
	wait(NULL);
	
}

void lavoroBenzinaioFiglio(){
	printf("\n lavoro figlio benzinaio inizia - 144 \n");
	int coda2 = crea_coda(0615);	//creo la coda messaggi
	//perror("acquisizione coda 2  143");
	//recupero la memoria condivisa
	int *puntMem;
	int shmid;
	shmid = shmget(0700, 10*sizeof(int),0666);
	puntMem=(int *) shmat(shmid, NULL,0666);	
	int idSemPompa2 = semget(0603, 1, 0);
	int idSemPienoFatto2 = semget(0605,1,0);
	int idSemSoldiInviati2 = semget(0607,1,0);
	int idSemVattene2 = semget(0611,1,0);
	int idSemMutexCassa = semget(0612,1,0);
	int idNumMa = semget(0616,1,0);
	int idSemPompa1 = semget(0602, 1, 0);
	
	/*int numeroMacchineLocale;
	waitSem(idNumMa);
	printf("\n numero  : %d \n", puntMem[2]);
		numeroMacchineLocale = puntMem[2];
	signalSem(idNumMa);*/

	while(1){
		
		printf("\n WHILE figlio benzinaio \n");
		waitSem(idNumMa);
			if(puntMem[2] == 0 )
				signalSem(idSemPompa2);
		signalSem(idNumMa);
		waitSem(idSemPompa2);
			waitSem(idNumMa);
			printf("\n valore prima if figlio %d \n", puntMem[2]);
			if(puntMem[2] == 0 ) {
				printf("\n entrato if figlio\n");
				signalSem(idNumMa);
				break;
			} 
			signalSem(idNumMa);
				printf("\n numero macchine rimaste prima del controllo figlio %d \n", puntMem[2]);
				struct my_msg *m=malloc(sizeof(struct my_msg));
				int importoDaChiedere = rand() % 100;
				scrivi_msg(coda2,m,importoDaChiedere);
				signalSem(idSemPienoFatto2);
					waitSem(idSemSoldiInviati2);	
						ricevi_msg(coda2,m);
						int soldiPresi = m->mtext;
					
					signalSem(idSemVattene2); 
				
					printf("\n fatta la vattene 2 \n");
				waitSem(idSemMutexCassa);
						puntMem[0] = puntMem[0]+soldiPresi;
				signalSem(idSemMutexCassa);
				waitSem(idNumMa);
					puntMem[2]=puntMem[2]-1;
					printf("\n numero macchine rimaste  figlio: %d \n", puntMem[2]);
				if(puntMem[2] ==0) {
						signalSem(idSemPompa2);
						signalSem(idSemPompa1);
						signalSem(idNumMa);
						break;
						}
				signalSem(idNumMa);
		//printf("\nsuperata signal num macchine figlio\n");
		
	}
	printf("\nfiglio uscito dal while\n");
		signalSem(idSemPompa1);	
	

	exit(0);
}

int main(){
	
	pid_t pidRestituito; /* process id */
	pidRestituito = fork();
	
	if (pidRestituito < 0){
		printf("!!errore fork benzinaio, EXIT \n");
		exit(1);
	}else{
		
	if (pidRestituito == 0)	//figlio (la stazione di servizio è a conduzione familiare)
		lavoroBenzinaioFiglio();
	else	//padre
		lavoroBenzinaioPadre();
	}
	
	exit(0);
}
