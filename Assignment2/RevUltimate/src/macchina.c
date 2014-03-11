#include "tutto.h"

void lavoroAuto();
void erroreUscitaPompa1();
void erroreUscitaPompa2();

int main(){
	//recupero memoria condivisa che mi serve a controllare quante fork devo fare poichè il numero di macchine da creare è settato
	// nel file sem_mem.c
	int shmid = shmget(MEM_CONDIVISA, 10*sizeof(int),0);
	if (shmid<0)
		perror ("shmget MEM_CONDIVISA: ");
	int *puntMem=(int *) shmat(shmid, NULL, 0);
	if (puntMem<0)
		perror ("shmat memoria condivisa: ");
	
	int i = 0;
	int numeroAuto = puntMem[2];
	
	while(i < numeroAuto){
		//fork
		pid_t pidRestituito; /* process identifier */
		pidRestituito = fork();
		if (pidRestituito < 0){
			perror("Fork macchina: ");
			exit(-1);
		}
		if (pidRestituito == 0){	//figlio
			lavoroAuto();
		}else{	//proseguo a creare auto
			i++;
		}
	}
	
	/*faccio wait sui miei figli auto, in attesa che terminino
	gestendo il valore di ritorno e in caso affermativo segnalo l'errore*/
	int k;
	int *valoreRitorno=NULL;
	int boolErrore=0;
	for (k=0; k<numeroAuto; k++){
		wait(valoreRitorno);
		if (valoreRitorno<0)
			boolErrore=1;
		}
	if (boolErrore)
		exit(-1);
	else
		exit(0);
}

/*CODICE AUTO*/
void lavoroAuto(){
	printf("\nAuto: inizio a lavorare, ho PID: %d\n",getpid());
	
	//recupero code
	int coda1=crea_coda(CODA_MESS1);
	if(coda1<0){
		printf("errore acquisizione coda1");
		exit(-1);
	}
	int coda2=crea_coda(CODA_MESS2);
	if(coda2<0){
		printf("errore acquisizione coda2");
		exit(-1);
	}	
	
	//recupero memoria condivisa
	int shmid = shmget(MEM_CONDIVISA, 10*sizeof(int),0);
	if (shmid<0){
		perror ("shmget MEM_CONDIVISA: ");
		exit(-1);
		}
	/*collego la memoria condivisa al mio spazio di indirizzamento e recupero il 
	relativo puntatore*/
	int *puntMem=(int *) shmat(shmid, NULL, 0);
	if (puntMem<0){
		perror ("shmat memoria condivisa: ");
		exit(-1);
		}
		
	//recupero semafori
	int mutex_pompa1libera = semget(SEM_POMPA1LIBERA,1,0);
	if (mutex_pompa1libera<0){
		perror("semget mutex_pompa1libera: ");
		exit(-1);
	}
	int idSemStazione = semget(SEM_STAZIONE,1,0);
	if (idSemStazione<0){
		perror("semget idSemStazione: ");
		exit(-1);
	}
	
	//variabile per controllare quale sia la pompa libera
	int p=0;	
	
	//Semaforo per entrare nella stazione
	waitSem(idSemStazione);
	
	//mutex per accedere alla variabile pompaLibera
	waitSem(mutex_pompa1libera);
	if (puntMem[1]==1){	//se la pompa1 è libera la setto a 0 (cioè occupata), ponendo p=1 per entrarci
		puntMem[1] = 0;
		p=1;
	}
	signalSem(mutex_pompa1libera);	//fine mutex
	
	if (p==1){ //cioè se la pompa1 è libera
		printf("Auto ( %d ): sono arrivato alla pompa1\n", getpid());

		//acquisisco i semafori	
		int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
		if (idSemPompa1<0){
			perror("semget idSemPompa1: ");
			erroreUscitaPompa1(mutex_pompa1libera,puntMem,idSemStazione);
			}
			
		int idSemPienoFatto1 = semget(SEM_PIENOFATTO1,1,0);
		if (idSemPienoFatto1<0){
			perror("semget idSemPienoFatto1: ");
			erroreUscitaPompa1(mutex_pompa1libera,puntMem,idSemStazione);
			}
		
		int idSemSoldiInviati1 = semget(SEM_SOLDIINVIATI1,1,0);
		if (idSemSoldiInviati1<0){
			perror ("semget idSemSoldiInviati1: ");
			erroreUscitaPompa1(mutex_pompa1libera,puntMem,idSemStazione);
			}
			
		int idSemVattene1 = semget(SEM_VATTENE1,1,0);
		if (idSemVattene1<0){
			perror ("semget idSemVattene1: ");
			erroreUscitaPompa1(mutex_pompa1libera,puntMem,idSemStazione);
			}
			
		//indico al benzinaio che sono arrivato
		signalSem(idSemPompa1);
		
		//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		waitSem(idSemPienoFatto1);	
		
		//recupero il valore dell'importo dalla coda messaggi1
		struct my_msg *m=malloc(sizeof(struct my_msg));
		ricevi_msg(coda1,m);
		printf("Auto ( %d ): devo pagare: %d\n", getpid(), m->mtext);
		
		//invio soldi su coda messaggi1
		scrivi_msg(coda1,m,m->mtext);
		
		//segnalo al benzinaio che li ho inviati
		signalSem(idSemSoldiInviati1);	
		
		//aspetto che il benzinaio mi segnali che ha recuperato i soldi
		waitSem(idSemVattene1);	
		
		//resetto il controllo per pompa1 a libera
		waitSem(mutex_pompa1libera);//mutex
		puntMem[1]=1;
		signalSem(mutex_pompa1libera);//fine mutex
	}
	else{	//la pompa1 è occupata allora entro in nella seconda
		printf("Auto ( %d ): sono arrivato alla pompa2\n", getpid());

		//recupero semafori
		int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
		if (idSemPompa2<0){
			perror("semget idSemPompa2: ");
			erroreUscitaPompa2(idSemStazione);
			}
		int idSemPienoFatto2 = semget( SEM_PIENOFATTO2,1,0);
		if (idSemPienoFatto2<0){
			perror("semget idSemPienoFatto2: ");
			erroreUscitaPompa2(idSemStazione);
			}
		int idSemSoldiInviati2 = semget(SEM_SOLDIINVIATI2,1,0);
		if (idSemSoldiInviati2<0){
			perror("smget idSemSoldiInviati2: ");
			erroreUscitaPompa2(idSemStazione);
			}
		int idSemVattene2 = semget(SEM_VATTENE2,1,0);
		if (idSemVattene2<0){
			perror ("semget idSemVattene2: ");
			erroreUscitaPompa2(idSemStazione);
		}
		
		//avverto il benzinaio che sono arrivato
		signalSem(idSemPompa2);	
		
		//aspetto che il benzinaio finisca il lavoro e mi invii l'importo
		waitSem(idSemPienoFatto2);	
		
		//recupero messaggio da coda2
		struct my_msg *m1=malloc(sizeof(struct my_msg));
		ricevi_msg(coda2,m1);
		printf("Auto ( %d ): devo pagare: %d\n", getpid(), m1->mtext);
		
		//invio soldi tramite coda2
		scrivi_msg(coda2,m1,m1->mtext);
		
		//indico al benzinaio che ho inviato i soldi
		signalSem(idSemSoldiInviati2);	
		
		//aspetto che il benzinaio mi segnali che ha recuperato i soldi
		waitSem(idSemVattene2); 
	}
	
	//Segnalo l'uscita dal semaforo della stazione
	signalSem(idSemStazione);
		
	printf("Auto ( %d ): TERMINATA\n",getpid());
	
	exit(0);
}

void erroreUscitaPompa1(int mutex_pompa1libera, int *puntMem, int idSemStazione){
	//resetto il controllo per pompa1 a libera
	waitSem(mutex_pompa1libera);//mutex
	puntMem[1]=1;
	signalSem(mutex_pompa1libera);//fine mutex
	//Segnalo l'uscita dal semaforo della stazione
	signalSem(idSemStazione);
	exit(-1);
	}

void erroreUscitaPompa2(int idSemStazione){
	//Segnalo l'uscita dal semaforo della stazione
	signalSem(idSemStazione);
	exit(-1);
	}
