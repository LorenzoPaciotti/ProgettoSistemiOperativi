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
		printf("dato ricevuto %d  da pid %d \n",a->mtext,getpid());
		
}

/*CODICE BENZINAIO*/
int leggiNumeroMacchineRimaste(int *puntMem){	//controllo del while nel lavoro benzinaio
	int idNumMa = semget(0616,1,0);
	waitSem(idNumMa);
		printf("\n numero : %d \n", puntMem[2]);
			int ciao = puntMem[2];
	signalSem(idNumMa);
	return ciao;
}

void lavoroBenzinaioPadre(){
	//~ printf("\n lavoro padre benzinaio inizia - 93 \n");
	int coda1 = crea_coda(0614);	//creo la coda di messaggi
	//~ perror("acquisizione coda 2  95");
	//recupero la memoria condivisa
	int *puntMem;
	int shmid;
	shmid = shmget(0700, 1,0666 );
	//~ perror("memoria acquisita");
	puntMem=(int *) shmat(shmid, NULL, 0666);
	//~ perror("fine attach");
	//recupero semafori
	int idSemPompa1 = semget(0602, 1, 0);
	//~ int idSemPompa2 = semget(0603, 1, 0);
	//~ perror("semget - 106");
	int idSemPienoFatto1 = semget(0604,1,0);
	//~ perror("semget - 108");
	int idSemSoldiInviati1 = semget(0606,1,0);
	//~ perror("semget - 110");
	int idSemVattene1 = semget(0610,1,0);
	//~ perror("semget - 112");
	int idSemMutexCassa = semget(0612,1,0);
	//~ perror("semget - 114");
	int idNumMa = semget(0616,1,0);
	//~ perror("semget - 163");
//	int mutex_pompa1libera = semget(0613,1,0);
	
	
		int numeroMacchineLocale;
	//~ while(1){
		waitSem(idNumMa);
		//~ printf("\n numero macchine locale padre: %d \n", puntMem[2]);
		numeroMacchineLocale = puntMem[2];
		signalSem(idNumMa);
		
		while(puntMem[3] == 0 && numeroMacchineLocale>0){
		
		signalSem(idNumMa);
		printf("\n inzio corpo WHILE padre benzinaio \n");
		
		waitSem(idSemPompa1);
		//~ perror("waitSem pompa1 166");
		//*metti benzina*
		//invio importo su msg1
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda1,m,importoDaChiedere);
			printf("\n importo da pagare inviato padre= %d \n",m->mtext);

		//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
		signalSem(idSemPienoFatto1); 
		waitSem(idSemSoldiInviati1); //aspetta che la macchina gli invii i soldi
		//prende i soldi dalla msg queue
			ricevi_msg(coda1,m);
			int soldiPresi = m->mtext;
		
		signalSem(idSemVattene1); //indica alla macchina che ha recuperato i soldi
		
		//deposito soldi nella cassa (memoria condivisa)
		waitSem(idSemMutexCassa); //wait(mutex_cassa)
			puntMem[0] = puntMem[0]+soldiPresi;
		signalSem(idSemMutexCassa);//signal(mutex_cassa)
		
		waitSem(idNumMa);
		puntMem[2]=puntMem[2]-1;
		if(puntMem[2] == 0)
			puntMem[3] = 1; //setto fine giornata
		//Modifico il numero di macchine e sovrascrivo la var locale per fare di nuovo il controllo while
		//~ waitSem(idNumMa); //semaforo mutex
				//~ 
				//~ printf("\n numero macchine rimaste PADRE: %d \n", puntMem[2]);
				//~ numeroMacchineLocale = puntMem[2];
		//~ signalSem(idNumMa);
		
		printf("\n fine corpo WHILE padre benzinaio \n");
		
		}
		//divisione soldi	DA FARE
	printf("\npadre uscito dal while\n");
	//wait(NULL);
	exit(0);
}

void lavoroBenzinaioFiglio(){
	//~ printf("\n lavoro figlio benzinaio inizia - 144 \n");
	int coda2 = crea_coda(0615);	//creo la coda messaggi
	//~ perror("acquisizione coda 2  143");
	//recupero la memoria condivisa
	int *puntMem;
	int shmid;
	shmid = shmget(0700, 1,0666);
	//~ perror("\n memoria acquisita");
	puntMem=(int *) shmat(shmid, NULL,0666);
	//~ perror("fine attach");

	//recupero i semafori
	//~ int idSemPompa1 = semget(0602, 1, 0);
	int idSemPompa2 = semget(0603, 1, 0);
	//~ perror("semget - 159");
	int idSemPienoFatto2 = semget(0605,1,0);
	//~ perror("semget - 161");
	int idSemSoldiInviati2 = semget(0607,1,0);
	//~ perror("semget - 163");
	int idSemVattene2 = semget(0611,1,0);
	//~ perror("semget - 165");
	int idSemMutexCassa = semget(0612,1,0);
	//~ perror("semget - 167");
	int idNumMa = semget(0616,1,0);
	//~ perror("semget - 163");
	
	
	
	
	int numeroMacchineLocale;
	//~ while(1){
		waitSem(idNumMa);
		printf("\n numero macchine locale figlio prima del while: %d \n", puntMem[2]);
		numeroMacchineLocale = puntMem[2];
		signalSem(idNumMa);
		
	while(puntMem[3] == 0 && numeroMacchineLocale>0){
			
		signalSem(idNumMa);
		printf("\n inizio corpo WHILE figlio benzinaio \n");
		
		waitSem(idSemPompa2);
		//~ perror("waitSem pompa2 166");
		//metti benzina
		//invio importo su msg2
		struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda2,m,importoDaChiedere);
			printf("\n importo da pagare inviato figlio= %d",m->mtext);
		//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
		signalSem(idSemPienoFatto2);
		waitSem(idSemSoldiInviati2);	//aspetta che la macchina gli invii i soldi
		//prendo i soldi dalla msg queue
		ricevi_msg(coda2,m);
		int soldiPresi = m->mtext;
		
		signalSem(idSemVattene2); //indica alla macchina che ha recuperato i soldi
		
		//deposito soldi nella cassa (memoria condivisa)
		waitSem(idSemMutexCassa);
			perror("\nFIGLIO entrato in cassa - 184: ");
			puntMem[0] = puntMem[0]+soldiPresi;
		signalSem(idSemMutexCassa);
		
		waitSem(idNumMa);
		puntMem[2]=puntMem[2]-1;
		if(puntMem[2] == 0)
			puntMem[3] = 1; //setto fine giornata
		
		//Modifico il numero di macchine e sovrascrivo la var locale per fare di nuovo il controllo while
		//~ waitSem(idNumMa);
				//~ 
				//~ printf("\n numero macchine rimaste FIGLIO: %d \n", puntMem[2]);
				//~ numeroMacchineLocale = puntMem[2];
		//~ signalSem(idNumMa);
		//~ printf("\nsuperata signal num macchine figlio\n");
		//~ 
		//~ printf("\n fine corpo WHILE figlio benzinaio \n");
	//~ else {
			//~ signalSem(idNumMa);
			//~ signalSem(idSemPompa1);
			//~ break;
		//~ }
	//~ }
	
	}
	printf("\nfiglio uscito dal while\n");
	//divisione soldi	DA FARE
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
