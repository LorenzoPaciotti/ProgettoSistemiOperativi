#include "tutto.h"

void lavoroBenzinaioPadre();
void lavoroBenzinaioFiglio();

int main(){
	
	pid_t pidRestituito; /* process id */
	pidRestituito = fork();
	if (pidRestituito < 0){
		printf("!!errore fork benzinaio, EXIT\n");
		exit(-1);
	}else{
		
	if (pidRestituito == 0)	//figlio (la stazione di servizio è a conduzione familiare)
		lavoroBenzinaioFiglio();
	else	//padre
		lavoroBenzinaioPadre();
	}
	wait(NULL);
	exit(0);
}

void lavoroBenzinaioPadre(){
	printf("\nlavoro padre benzinaio inizia\n");
	
	//creo la coda di messaggi
	int coda1 = crea_coda(CODA_MESS1);	
	
	//recupero la memoria condivisa
	int shmid = shmget(MEM_CASSA, 10*sizeof(int),0666 );
	if (shmid<0)
		perror("shmget memoria condivisa: ");
	int *puntMem=(int *) shmat(shmid, NULL, 0666);
	if (puntMem<0)
		perror("shmat memoria condivisa: ");
	
	//recupero semafori
	int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
	if (idSemPompa1<0)
		perror("semget idSemPompa1: ");
	int idSemPienoFatto1 = semget(SEM_PIENOFATTO1,1,0);
	if(idSemPienoFatto1<0)
		perror("semget idSemPienoFatto1: ");
	int idSemSoldiInviati1 = semget(SEM_SOLDIINVIATI1,1,0);
	if (idSemSoldiInviati1<0)
		perror("semget idSemSoldiInviati1: ");
	int idSemVattene1 = semget(SEM_VATTENE1,1,0);
	if (idSemVattene1<0)
		perror("semget idSemVattene1: ");
	int idSemMutexCassa = semget(SEM_MUTEXCASSA,1,0);
	if (idSemMutexCassa<0)
		perror("semget idSemMutexCassa: ");
	int idNumMa = semget(SEM_NUMEROMACCHINE,1,0);
	if(idNumMa<0)
		perror("semget idNumMa: ");
	int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
	if(idSemPompa2<0)
		perror("semget idSemPompa2");
	
	waitSem(idNumMa);
	while(puntMem[2]>0){
		signalSem(idNumMa);
		
		printf("\nWHILE padre benzinaio \n");
		
		waitSem(idSemPompa1);
		if (puntMem[3]==1){
			printf("il padre benzinaio ha breccato\n");
			break;
		}
		else{
			//*metti benzina*
			//invio importo su msg1
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda1,m,importoDaChiedere);
			printf("importo da pagare inviato padre= %d \n",m->mtext);

			//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
			signalSem(idSemPienoFatto1); 
			
			//aspetta che la macchina gli invii i soldi
			waitSem(idSemSoldiInviati1); 
		
			//prende i soldi dalla msg queue
			ricevi_msg(coda1,m);
			int soldiPresi = m->mtext;
		
			//indica alla macchina che ha recuperato i soldi
			signalSem(idSemVattene1);
			
			//deposito soldi nella cassa (memoria condivisa)
			waitSem(idSemMutexCassa); //wait(mutex_cassa)
			puntMem[0] = puntMem[0]+soldiPresi;
			signalSem(idSemMutexCassa);//signal(mutex_cassa)
				
			//Modifico il numero di macchine
			waitSem(idNumMa); //semaforo mutex
			puntMem[2]=puntMem[2]-1;
			printf("numero macchine rimaste: %d \n", puntMem[2]);
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
	printf("Sono il padre e ho guadagnato %d\n",paga);
}

void lavoroBenzinaioFiglio(){
	printf("lavoro figlio benzinaio inizia\n");
	
	//creo la coda messaggi
	int coda2 = crea_coda(CODA_MESS2);
	
	//recupero la memoria condivisa
	int shmid = shmget(MEM_CASSA, 10*sizeof(int),0666);
	if (shmid<0)
		perror("shmget memoria condivisa: ");
	int puntMem=(int *) shmat(shmid, NULL,0666);
	if (puntMem<0)
		perror("shmat memoria condivisa: ");

	//recupero i semafori
	int idSemPompa2 = semget(SEM_POMPA2, 1, 0);
	if (idSemPompa2<0)
		perror("semget idSemPompa2: ");
	int idSemPienoFatto2 = semget(SEM_PIENOFATTO2,1,0);
	if(idSemPienoFatto2<0)
		perror("semget idSemPienoFatto2: ");
	int idSemSoldiInviati2 = semget(SEM_SOLDIINVIATI2,1,0);
	if (idSemSoldiInviati2<0)
		perror("semget idSemSoldiInviati2: ");
	int idSemVattene2 = semget(SEM_VATTENE2,1,0);
	if (idSemVattene2<0)
		perror("semget idSemVattene2: ");
	int idSemMutexCassa = semget(SEM_MUTEXCASSA,1,0);
	if (idSemMutexCassa<0)
		perror("semget idSemMutexCassa: ");
	int idNumMa = semget(SEM_NUMEROMACCHINE,1,0);
	if (idNumMa<0)
		perror("semget idNumMa: ");
	int idSemPompa1 = semget(SEM_POMPA1, 1, 0);
	if (idSemPompa1<0)
		perror("semget idSemPompa1: ");
	
	waitSem(idNumMa);
	while(puntMem[2]>0){
		signalSem(idNumMa);
		printf("\n WHILE figlio benzinaio \n");
		
		waitSem(idSemPompa2);
		if (puntMem[3]==1){
			printf("figlio breccato\n");
			break;
			}
		else{
			//metti benzina
			//invio importo su msg2
			struct my_msg *m=malloc(sizeof(struct my_msg));
			int importoDaChiedere = rand() % 100;
			scrivi_msg(coda2,m,importoDaChiedere);
			printf("importo da pagare inviato figlio= %d",m->mtext);
			
			//segnala alla macchina che ha finito di fare il pieno e di conseguenza che gli ha inviato il costo del pieno
			signalSem(idSemPienoFatto2);
			
			//aspetta che la macchina gli invii i soldi
			waitSem(idSemSoldiInviati2);
			
			//prendo i soldi dalla msg queue
			ricevi_msg(coda2,m);
			int soldiPresi = m->mtext;
			
			//indica alla macchina che ha recuperato i soldi e se ne può andare
			signalSem(idSemVattene2);
			
			//deposito soldi nella cassa (memoria condivisa)
			waitSem(idSemMutexCassa);
			perror("\nentrato in cassa - 184: ");
			puntMem[0] = puntMem[0]+soldiPresi;
			signalSem(idSemMutexCassa);
			
			//Modifico il numero di macchine
			waitSem(idNumMa);
				puntMem[2]=puntMem[2]-1;
				printf("numero macchine rimaste: %d \n", puntMem[2]);
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
	printf("Sono il figlio e ho guadagnato %d\n",paga);
	
	exit(0);
}