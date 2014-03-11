#include "tutto.h"

void lavoroBenzinaioPadre();
void lavoroBenzinaioFiglio();

int main(){
	
	pid_t pidRestituito; /* process id */
	pidRestituito = fork();
	if (pidRestituito < 0){
		printf("errore fork benzinaio, EXIT\n");
		exit(-1);
	}else{
		if (pidRestituito == 0)	//figlio (la stazione di servizio è a conduzione familiare)
			lavoroBenzinaioFiglio();
		else	//padre
			lavoroBenzinaioPadre();
	}
	
	int *valoreRitornoExitFiglio = NULL;
	wait(valoreRitornoExitFiglio);
	if(valoreRitornoExitFiglio < 0)
		exit(-1);
	else
		exit(0);
}

void lavoroBenzinaioPadre(){
	printf("\nPadre: Inizio a lavorare\n");
	
	//creo la coda di messaggi
	int coda1 = crea_coda(CODA_MESS1);	
	if(coda1<0){
		printf("errore acquisizione coda messaggi 1 padre benzinaio");
		wait(NULL); //c'è stato un errore syscall, ma mi metto comunque in attesa di mio figlio in modo da non lasciarlo zombie
		exit(-1);
	}
	
	//recupero la memoria condivisa
	int shmid = shmget(MEM_CONDIVISA, 10*sizeof(int),0666 );
	if (shmid<0){
		perror("shmget memoria condivisa: ");
		wait(NULL);
		exit(-1);
	}
	//attach della memoria condivisa al mio spazio di indirizzamento
	int *puntMem=(int *) shmat(shmid, NULL, 0666);
	if (puntMem<0){
		perror("shmat memoria condivisa: ");
		wait(NULL);
		exit(-1);
	}
	
	//recupero semafori
	int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
	if (idSemPompa1<0){
		perror("semget idSemPompa1: ");
		wait(NULL);
		exit(-1);
	}
	int idSemPienoFatto1 = semget(SEM_PIENOFATTO1,1,0);
	if(idSemPienoFatto1<0){
		perror("semget idSemPienoFatto1: ");
		wait(NULL);
		exit(-1);
	}
	int idSemSoldiInviati1 = semget(SEM_SOLDIINVIATI1,1,0);
	if (idSemSoldiInviati1<0){
		perror("semget idSemSoldiInviati1: ");
		wait(NULL);
		exit(-1);
	}
	int idSemVattene1 = semget(SEM_VATTENE1,1,0);
	if (idSemVattene1<0){
		perror("semget idSemVattene1: ");
		wait(NULL);
		exit(-1);
	}
	int idSemMutexCassa = semget(SEM_MUTEXCASSA,1,0);
	if (idSemMutexCassa<0){
		perror("semget idSemMutexCassa: ");
		wait(NULL);
		exit(-1);
	}
	int idNumMa = semget(SEM_NUMEROMACCHINE,1,0);
	if(idNumMa<0){
		perror("semget idNumMa: ");
		wait(NULL);
		exit(-1);
	}
	int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
	if(idSemPompa2<0){
		perror("semget idSemPompa2");
		wait(NULL);
		exit(-1);
	}
	
	waitSem(idNumMa);
	while(puntMem[2]>0){	//while rispetto al numero di macchine
		signalSem(idNumMa);
		printf("Padre: Aspetto la macchina\n");
		waitSem(idSemPompa1);	//aspetto che arrivi una macchina
		if (puntMem[3]==1){	//se la zona di memoria condivisa 3 è uguale a 1 allora sono finite le macchine
			printf("\nPadre: Il benzinaio figlio mi segnala che la giornata è finita\n");
			break;
		}
		else{
			//*metti benzina*

			//invio importo su msg1
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda1,m,importoDaChiedere);
			printf("\nPadre: importo da pagare inviato= %d \n",m->mtext);

			//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
			signalSem(idSemPienoFatto1); 
			
			//aspetta che la macchina gli invii i soldi
			waitSem(idSemSoldiInviati1); 
			
			//prende i soldi dalla msg queue
			ricevi_msg(coda1,m);
			int soldiPresi = m->mtext;
			printf("Padre: Soldi ricevuti: %d \n", soldiPresi);
			
			//indica alla macchina che ha recuperato i soldi
			signalSem(idSemVattene1);
			
			//deposito soldi nella cassa (memoria condivisa)
			waitSem(idSemMutexCassa); //wait(mutex_cassa)
			printf("Padre: entrato in cassa\n");
			puntMem[0] = puntMem[0]+soldiPresi;
			signalSem(idSemMutexCassa);//signal(mutex_cassa)

			//Modifico il numero di macchine
			waitSem(idNumMa); //semaforo mutex
			puntMem[2]=puntMem[2]-1;
			printf("Padre: numero macchine rimaste: %d \n", puntMem[2]);
			}
		}
	signalSem(idNumMa);
	
	//setto la variabile fine giornata a TRUE
	puntMem[3]=1;
	
	//segnalo all'altro benzinaio che sono finite le macchine
	signalSem(idSemPompa2);
	
	//divido gli incassi della giornata e prendo la mia parte
	waitSem(idSemMutexCassa);
	int paga=puntMem[0]/2;
	signalSem(idSemMutexCassa);
	printf("Padre: ho guadagnato: %d\n",paga);
}

void lavoroBenzinaioFiglio(){
	printf("Figlio: inizio a lavorare\n");
	
	//creo la coda messaggi
	int coda2 = crea_coda(CODA_MESS2);
	if(coda2<0){
		printf("errore acquisizione coda messaggi 2 figlio benzinaio");
		exit(-1);
	}
	
	//recupero la memoria condivisa
	int shmid = shmget(MEM_CONDIVISA, 10*sizeof(int),0666);
	if (shmid<0){
		perror("shmget memoria condivisa: ");
		exit(-1);
	}
	//attach della memoria condivisa al mio spazio di indirizzamento
	int *puntMem=(int *) shmat(shmid, NULL,0666);
	if (puntMem<0){
		perror("shmat memoria condivisa: ");
		exit(-1);
	}

	//recupero i semafori
	int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
	if (idSemPompa2<0){
		perror("semget idSemPompa2: ");
		exit(-1);
	}
	int idSemPienoFatto2 = semget(SEM_PIENOFATTO2,1,0);
	if(idSemPienoFatto2<0){
		perror("semget idSemPienoFatto2: ");
		exit(-1);
	}
	int idSemSoldiInviati2 = semget(SEM_SOLDIINVIATI2,1,0);
	if (idSemSoldiInviati2<0){
		perror("semget idSemSoldiInviati2: ");
		exit(-1);
	}
	int idSemVattene2 = semget(SEM_VATTENE2,1,0);
	if (idSemVattene2<0){
		perror("semget idSemVattene2: ");
		exit(-1);
	}
	int idSemMutexCassa = semget(SEM_MUTEXCASSA,1,0);
	if (idSemMutexCassa<0){
		perror("semget idSemMutexCassa: ");
		exit(-1);
	}
	int idNumMa = semget(SEM_NUMEROMACCHINE,1,0);
	if (idNumMa<0){
		perror("semget idNumMa: ");
		exit(-1);
	}
	int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
	if (idSemPompa1<0){
		perror("semget idSemPompa1: ");
		exit(-1);
	}
	
	waitSem(idNumMa);
	while(puntMem[2]>0){
		signalSem(idNumMa);
		printf("Figlio: aspetto l'auto\n");
		
		waitSem(idSemPompa2);
		if (puntMem[3]==1){
			printf("Figlio: Il benzinaio padre mi ha detto che la giornata è finita\n");
			break;
			}
		else{
			//*metti benzina*

			//invio importo su msg2
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda2,m,importoDaChiedere);
			printf("Figlio: importo da pagare inviato= %d\n",m->mtext);
			
			//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
			signalSem(idSemPienoFatto2);
			
			//aspetta che la macchina gli invii i soldi
			waitSem(idSemSoldiInviati2);
			
			//prendo i soldi dalla msg queue
			ricevi_msg(coda2,m);
			int soldiPresi = m->mtext;
			printf("Figlio: ho ricevuto i soldi: %d\n", soldiPresi);

			//indica alla macchina che ha recuperato i soldi e se ne può andare
			signalSem(idSemVattene2);
			
			//deposito soldi nella cassa (memoria condivisa)
			waitSem(idSemMutexCassa);
			printf("Figlio: entrato in cassa\n");
			puntMem[0] = puntMem[0]+soldiPresi;
			signalSem(idSemMutexCassa);

			//Modifico il numero di macchine
			waitSem(idNumMa);
				puntMem[2]=puntMem[2]-1;
				printf("Figlio: numero macchine rimaste: %d \n", puntMem[2]);
		}
	}
	signalSem(idNumMa);
	
	//setto la variabile fine giornata a TRUE
	puntMem[3]=1;
	
	//segnalo all'altro benzinaio che sono finite le macchine
	signalSem(idSemPompa1);
	
	//divisione soldi
	waitSem(idSemMutexCassa);
	int paga=puntMem[0]/2;
	signalSem(idSemMutexCassa);
	printf("Figlio: ho guadagnato %d\n",paga);
	
	exit(0);
}
