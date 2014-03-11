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
	wait_buf.sem_flg = SEM_UNDO;
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
	signal_buf.sem_flg = SEM_UNDO;
	if(semop(semid, &signal_buf, 1) == -1) {
		perror("signalSem fallita");
		exit(1);
	}
	return(0);
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
			exit(4);
		}
}

/*CODICE BENZINAIO*/
void lavoroBenzinaioPadre(){
	printf("\n lavoro padre benzinaio inizia \n");
	//creo la coda di messaggi
	int coda1 = crea_coda(0614);
	
	//recupero la memoria condivisa
	int *puntMem, shmid;
	shmid = shmget(0700, 10*sizeof(int),0666 );
	puntMem=(int *) shmat(shmid, NULL, 0666);
	
	//recupero semafori
	int idSemPompa1 = semget(0602, 1, 0);
	int idSemPienoFatto1 = semget(0604,1,0);
	int idSemSoldiInviati1 = semget(0606,1,0);
	int idSemVattene1 = semget(0610,1,0);
	int idSemMutexCassa = semget(0612,1,0);
	int idNumMa = semget(0616,1,0);
	int idSemPompa2 = semget(0603, 1, 0);
	
	int bool = 1;
	while(bool == 1){
		printf("\n WHILE padre benzinaio \n");
		
		waitSem(idSemPompa1);
		printf("\n superata wait pompa1 \n");
		waitSem(idNumMa);
			if(puntMem[2] == 0 ){
				printf("\n padre entrato nell'if \n");
				signalSem(idNumMa);
				bool = 0;
			} else {
			printf("\n padre entrato nell'else \n");
			
		signalSem(idNumMa);
			
		//*metti benzina*
		
		//invio importo su msg1
		struct my_msg *m=malloc(sizeof(struct my_msg));
		int importoDaChiedere = rand() % 100;
		scrivi_msg(coda1,m,importoDaChiedere);

		//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo
		signalSem(idSemPienoFatto1); 
		waitSem(idSemSoldiInviati1); //aspetta che la macchina gli invii i soldi
		printf("\n superata wait soldiinviati1 \n");
		//prende i soldi dalla msg queue
		ricevi_msg(coda1,m);
		printf("\n benzinaio padre ha ricevuto messaggio \n");
		int soldiPresi = m->mtext;
		
		signalSem(idSemVattene1); //indica alla macchina che ha recuperato i soldi
		printf("\n signal vattene1 inviato \n");
		//deposito soldi nella cassa (memoria condivisa)
		waitSem(idSemMutexCassa); //wait(mutex_cassa)
			puntMem[0] = puntMem[0]+soldiPresi;
		signalSem(idSemMutexCassa);//signal(mutex_cassa)
		printf("\n padre uscito dalla cassa \n");
		
		//Modifico il numero di macchine e sovrascrivo la var locale per fare di nuovo il controllo while
		waitSem(idNumMa); //semaforo mutex
			puntMem[2]=puntMem[2]-1;
			printf("\n numero macchine rimaste  padre: %d \n", puntMem[2]);
			if(puntMem[2] ==0) {
				printf("\n sono nell'if alla fine del while padre \n");
				signalSem(idSemPompa2);
				bool = 0;
			}
		signalSem(idNumMa);
		}
		
	}
	//divisione soldi	DA FARE
	
	printf("\npadre uscito dal while\n");
	wait(NULL);
}

void lavoroBenzinaioFiglio(){
	printf("\n lavoro figlio benzinaio inizia - 144 \n");
	
	//creo la coda messaggi
	int coda2 = crea_coda(0615);
	
	//recupero la memoria condivisa
	int *puntMem, shmid;
	shmid = shmget(0700, 10*sizeof(int),0666);
	puntMem=(int *) shmat(shmid, NULL,0666);

	//recupero i semafori
	int idSemPompa2 = semget(0603, 1, 0);
	int idSemPienoFatto2 = semget(0605,1,0);
	int idSemSoldiInviati2 = semget(0607,1,0);
	int idSemVattene2 = semget(0611,1,0);
	int idSemMutexCassa = semget(0612,1,0);
	int idNumMa = semget(0616,1,0);
	int idSemPompa1 = semget(0602, 1, 0);

	int bool = 1;
	while(bool == 1){
		
		printf("\n WHILE figlio benzinaio \n");
		
		waitSem(idSemPompa2);
		printf("\n superata wait pompa2 \n");
		waitSem(idNumMa);
		
		if(puntMem[2] == 0 ) {
			printf("\n figlio entrato nell'if \n");
			signalSem(idNumMa);
			bool = 0;
		} else {
			signalSem(idNumMa);
			printf("\n figlio entrato nell'else \n");
			//metti benzina
		
			//invio importo su msg2
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda2,m,importoDaChiedere);
		
			//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
			signalSem(idSemPienoFatto2);
			waitSem(idSemSoldiInviati2);	//aspetta che la macchina gli invii i soldi
			printf("\n superata wait soldiinviati2 \n");
		
			//prendo i soldi dalla msg queue
			ricevi_msg(coda2,m);
			printf("\n benzinaio figlio ha ricevuto messaggio \n");
			int soldiPresi = m->mtext;
			signalSem(idSemVattene2); //indica alla macchina che ha recuperato i soldi
			printf("\n signal vattene2 inviata \n");
			//deposito soldi nella cassa (memoria condivisa)
			waitSem(idSemMutexCassa);
			puntMem[0] = puntMem[0]+soldiPresi;
			signalSem(idSemMutexCassa);
			printf("\n figlio uscito dalla cassa \n");
			waitSem(idNumMa);
				puntMem[2]=puntMem[2]-1;
				printf("\n numero macchine rimaste  figlio: %d \n", puntMem[2]);
				if(puntMem[2] ==0) {
					printf("\n sono nell'if alla fine del while figlio \n");
					signalSem(idSemPompa1);
					bool = 0;
				}
			signalSem(idNumMa);
		}//fine else
	}//fine while
	printf("\nfiglio uscito dal while\n");
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
