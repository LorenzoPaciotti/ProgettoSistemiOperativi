#include "tutto.h"

/*CODICE AUTO*/
void lavoroAuto(){
	printf("\n ####### inizio lavoro auto con PID: %d #######",getpid());
	//recupero code
	int coda1=crea_coda(CODA_MESS1);
	int coda2=crea_coda(CODA_MESS2);
	//recupero memoria condivisa
	int *punt1;
	int shmid;
	shmid = shmget(MEM_CASSA, 10*sizeof(int),0);
	punt1=(int *) shmat(shmid, NULL, 0);
	//recupero semafori
	int mutex_pompa1libera = semget(SEM_POMPA1LIBERA,1,0);
	perror("mutex 105");
	int idSemStazione = semget(SEM_STAZIONE,1,0);
	perror("semget macchina 105");
	
	int p=0;	//variabile per controllare quale sia la pompa libera
	
	waitSem(idSemStazione);	//mutex stazione
	perror("waitsem 110");
	waitSem(mutex_pompa1libera);	//mutex
		perror("\nwaitsem mutex pompa1 111\n");
		if(punt1[1]==1){	//se la pompa1 è libera la setto a 0 (cioè occupata), ponendo p=1 per entrarci
			punt1[1] = 0;
			p=1;
		}
	signalSem(mutex_pompa1libera);//fine mutex
	
	if(p==1){
		//cioè se la pompa1 è libera
		//acquisisco i semafori	
		int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
		int idSemPienoFatto1 = semget(SEM_PIENOFATTO1,1,0);
		int idSemSoldiInviati1 = semget(SEM_SOLDIINVIATI1,1,0);
		int idSemVattene1 = semget(SEM_VATTENE1,1,0);
		
		signalSem(idSemPompa1);	//indico al benzinaio che sono arrivato
		perror("signal sem pompa 1 126");
		
		//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		waitSem(idSemPienoFatto1);	
		
		//recupero il valore dell'importo dalla coda messaggi1
		struct my_msg *m=malloc(sizeof(struct my_msg));
		ricevi_msg(coda1,m);
		perror("ricevi_messaggio 132");
		printf("\n importo da pagare ricevuto pompa 1= %d \n",m->mtext);
		
		//invio soldi su coda messaggi1
		scrivi_msg(coda1,m,m->mtext);
		signalSem(idSemSoldiInviati1);	//segnalo al benzinaio che li ho inviati
		
		//aspetto che il benzinaio mi segnali che ha recuperato i soldi
		waitSem(idSemVattene1);	
		
		//resetto il controllo per pompa1 a libera
		waitSem(mutex_pompa1libera);//mutex
		punt1[1]=1;
		signalSem(mutex_pompa1libera);//fine mutex
	}
	else{	
		//la pompa1 è occupata allora entro nella seconda
		//recupero semafori
		int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
		int idSemPienoFatto2 = semget( SEM_PIENOFATTO2,1,0);
		int idSemSoldiInviati2 = semget(SEM_SOLDIINVIATI2,1,0);
		int idSemVattene2 = semget(SEM_VATTENE2,1,0);
		
		//avverto il benzinaio che sono arrivato
		signalSem(idSemPompa2);	
		
		//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		waitSem(idSemPienoFatto2);	
		
		//recupero messaggio da coda2
		struct my_msg *m1=malloc(sizeof(struct my_msg));
		ricevi_msg(coda2,m1);
		printf("\nimporto da pagare ricevuto pompa 2= %d \n",m1->mtext);
		
		//invio soldi tramite coda2
		scrivi_msg(coda2,m1,m1->mtext);
		
		//indico al benzinaio che ho inviato i soldi
		signalSem(idSemSoldiInviati2);	
		
		//aspetto che il benzinaio mi segnali che ha recuperato i soldi
		waitSem(idSemVattene2); 
	}
	//mutex stazione fine
	signalSem(idSemStazione);
		
	printf("\n##########################MACCHINA TERMINATA(riga 162)(pid: %d)############################################ \n",getpid());
	
	exit(0);
}



int main(){
	//recupero memoria condivisa che mi serve a controllare quante fork devo fare poichè il numero di macchine da creare è settato
	// nel file sem_mem.c
	int *punt1;
	int shmid;
	shmid = shmget(MEM_CASSA, 10*sizeof(int),0);
	punt1=(int *) shmat(shmid, NULL, 0);
	
	int i = 0;
	
	int numeroAuto = punt1[2];
	while(i < numeroAuto){
		//fork
		pid_t pidRestituito; /* process identifier */
		pidRestituito = fork();
		
		if (pidRestituito < 0){
			printf("errore fork macchina !!\n");
			exit(-1);
		}
		if (pidRestituito == 0){	//figlio
			lavoroAuto();
		}else{	//proseguo a creare auto
			i++;
		}
	}
	int k;
	for (k=0; k<numeroAuto; k++)
		wait(NULL);
	exit(0);
}
