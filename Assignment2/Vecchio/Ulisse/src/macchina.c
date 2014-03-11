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
			perror("errore msgsnd RIGA 102 \n");
			exit(3);
			}
					
}

void ricevi_msg(int coda,struct my_msg *a){
	int mlen;
		if ( (mlen = msgrcv(coda, a,sizeof(int),0, MSG_NOERROR) == -1)){
			perror("errore nella msgrcv; RIGA 113\n");
			exit(4);}
		printf("\n dato ricevuto macchina: %d\n",a->mtext);
		
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

/* SEMAFORI */
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

/*CODICE AUTO*/
void lavoroAuto(){
	printf("####### inizio lavoro auto con PID: %d #######",getpid());
	//recupero code
	int coda1=crea_coda(0614);
	int coda2=crea_coda(0615);
	//recupero memoria condivisa
	int *punt1, shmid;
	shmid = shmget(0700, 10*sizeof(int),0);
	punt1=(int *) shmat(shmid, NULL, 0);
	//recupero semafori
	int mutex_pompa1libera = semget(0613,1,0);
	int idSemStazione = semget(0601,1,0);
	
	int p=0;	//variabile per controllare quale sia la pompa libera
	
	waitSem(idSemStazione);	//mutex stazione
	waitSem(mutex_pompa1libera);
		if(punt1[1] == 1 ){
			punt1[1] = 0;
			p=1;		
		}	
	signalSem(mutex_pompa1libera);//fine mutex
	
	if(p==1){	//cioè se la pompa1 è libera
		//acquisisco i semafori	
		int idSemPompa1 = semget(0602, 1, 0);
		int idSemPienoFatto1 = semget(0604,1,0);
		int idSemSoldiInviati1 = semget(0606,1,0);
		int idSemVattene1 = semget(0610,1,0);
		
		signalSem(idSemPompa1);	//indico al benzinaio che sono arrivato
		
		waitSem(idSemPienoFatto1);	//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		
		//recupero il valore dell'importo dalla coda messaggi1
			struct my_msg *m=malloc(sizeof(struct my_msg));
			ricevi_msg(coda1,m);
		//invio soldi su coda messaggi1
			scrivi_msg(coda1,m,m->mtext);
		signalSem(idSemSoldiInviati1);	//segnalo al benzinaio che li ho inviati
		
		waitSem(idSemVattene1);	
				printf("\n ho superato la wait di vattene1 \n");
//aspetto che il benzinaio mi segnali che ha recuperato i soldi
		//resetto il controllo per pompa1 a libera
		waitSem(mutex_pompa1libera);//mutex
			punt1[1]=1;
		signalSem(mutex_pompa1libera);//fine mutex
	}
	else{	//la pompa1 è occupata allora entro nella seconda
		//recupero semafori
		int idSemPompa2 = semget(0603, 1, 0);
		int idSemPienoFatto2 = semget( 0605,1,0);
		int idSemSoldiInviati2 = semget(0607,1,0);
		int idSemVattene2 = semget(0611,1,0);
		signalSem(idSemPompa2);	//avverto il benzinaio che sono arrivato
		
		waitSem(idSemPienoFatto2);	//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		
		//recupero messaggio da coda2
		struct my_msg *m1=malloc(sizeof(struct my_msg));
		ricevi_msg(coda2,m1);
		//invio soldi tramite coda2
		scrivi_msg(coda2,m1,m1->mtext);
		signalSem(idSemSoldiInviati2);
		//indico al benzinaio che ho inviato i soldi
		waitSem(idSemVattene2); //aspetto che il benzinaio mi segnali che ha recuperato i soldi
		printf("\n ho superato la wait di vattene2 \n");
	}//fine else
	
	signalSem(idSemStazione);//mutex stazione fine
	
	printf("\n##########################MACCHINA TERMINATA(riga 162)(pid: %d)############################################ \n",getpid());
	
	exit(0);
}



int main(){
	//recupero memoria condivisa che mi serve a controllare quante fork devo fare poichè il numero di macchine da creare è settato
	// nel file sem_mem.c
	int *punt1;
	int shmid;
	shmid = shmget(0700, 10*sizeof(int),0);
	punt1=(int *) shmat(shmid, NULL, 0);
	
	int i = 0;
	
	int numeroAuto = punt1[2];
	while(i < numeroAuto){
		//fork
		pid_t pidRestituito; /* process identifier */
		pidRestituito = fork();
		
		if (pidRestituito < 0){
			exit(1);
		} if (pidRestituito == 0){	//figlio
			lavoroAuto();
			break;
			}else{	//proseguo a creare auto
			i++;
			}
	}
	//aspetto le auto
	int k;
	for(k = 0;k<numeroAuto;k++)
		wait(NULL);
	
	exit(0);
}
