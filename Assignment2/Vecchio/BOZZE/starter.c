#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>

void asciiTitolo();
void asciiBenzinaio();
void asciiCredits();
void asciiMacchina();
void distruggiRisorse();

int main(){
	int* ritornoSemMem=NULL;

	asciiTitolo();
	char invio;
	printf("\n....premere un tasto per continuare...\n");
	scanf("%c",&invio);
	printf ("Start del programma \n ");
	
	//creo il processo sem_mem
	printf("creo il processo sem_mem......");
	pid_t sem_mem;
	sem_mem=fork();
	if (sem_mem<0){
		perror("fork sem_mem: ");
		exit(-1);
		}
	if (sem_mem==0){
		execl("./bin/sem_mem", "sem_mem", NULL, (char *) 0);
		}
	perror("exec sem_mem: ");
	wait(ritornoSemMem);
	if(ritornoSemMem<0){
		printf("errore nell'esecuzione di sem_men");
		distruggiRisorse();
		exit(-1);
		}
	printf("fatto \n");
	
	//creo il processo benzinaio
	asciiBenzinaio();
	 
	pid_t benzinaio;
	benzinaio=fork();
	if (benzinaio<0){
		perror("fork benzinaio: ");
		distruggiRisorse();
		exit(-1);
	}
	if (benzinaio==0){
		execl( "./bin/benzinaio", "benzinaio" ,NULL,(char *) 0);	
	}
	printf("fatto \n");
	perror("exec benzinaio: ");
	
	
	//creo il processo macchina
	asciiMacchina();
	
	pid_t macchina;
	macchina=fork();
	if(macchina<0){
		perror("fork macchina: ");		
		distruggiRisorse();
		wait(NULL);
		exit(-1);
		}
	if (macchina==0){
		execl("./bin/macchina","macchina",NULL,(char *) 0);
	}
	printf("fatto \n");
	perror("exec macchina: ");
	
	
	//debug scritte fighe
	asciiCredits();
	//debug scritte fighe
	
	
	//faccio le wait ai processi macchina e benzinaio
	wait(NULL);
	wait(NULL);
	
	distruggiRisorse();
	
	printf("...TERMINAZIONE DEL PROGRAMMA...");
	
                                  
	return 0;
}

void distruggiRisorse(){
	//aquisisco l'id dei semafori e li elimino
	printf("sto eliminando i semafori.......");
	
	//SEMPERM 0600
	int semPerm = semget(0600,0,1);
	if ((semctl(semPerm,0,IPC_RMID, 0))<0)
		perror("rimozione semPerm: ");
	
	//SEM_STAZIONE 0601
	int semStazione = semget(0601,0,1);
	if ((semctl(semStazione,0,IPC_RMID, 0))<0)
		perror("rimozione semStazione: ");
	
	//SEM_POMPA1 0602
	int semPompa1 = semget(0602,0,1);
	if ((semctl(semPompa1,0,IPC_RMID, 0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_POMPA2 0603
	int semPompa2 = semget(0603,0,1);
	if ((semctl(semPompa2,0,IPC_RMID, 0))<0)
		perror("rimozione semPompa1: ");
	
	//SEM_PIENOFATTO1 0604
	int semPienoFatto1 = semget(0604,0,1);
	if ((semctl(semPienoFatto1,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto1: ");
	
	//SEM_PIENOFATTO2 0605
	int semPienoFatto2 = semget(0605,0,1);
	if ((semctl(semPienoFatto2,0,IPC_RMID, 0))<0)
		perror("rimozione semPienoFatto2: ");
	
	//SEM_SOLDIINVIATI1 0606
	int semSoldiInviati1 = semget(0606,0,1);
	if ((semctl(semSoldiInviati1,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati: ");
	
	//SEM_SOLDIINVIATI2 0607
	int semSoldiInviati2 = semget(0607,0,1);
	if ((semctl(semSoldiInviati2,0,IPC_RMID, 0))<0)
		perror("rimozione semSoldiInviati2: ");
	
	//SEM_VATTENE1 0610
	int vattene1 = semget(0610,0,1);
	if ((semctl(vattene1,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene1: ");
	
	//SEM_VATTENE2 0611
	int vattene2 = semget(0611,0,1);
	if((semctl(vattene2,0,IPC_RMID, 0))<0)
		perror("rimozione semVattene2: ");
	
	//SEM_MUTEXCASSA 0612
	int mutexCassa = semget(0612,0,1);
	if((semctl(mutexCassa,0,IPC_RMID, 0))<0)
		perror("rimozione mutexCassa: ");
	
	//SEM_POMPA1LIBERA 0613
	int pompa1Libera = semget(0613,0,1);
	if((semctl(pompa1Libera,0,IPC_RMID, 0))<0)
		perror("rimozione sempompa1Libera: ");
	
	printf("fatto \n");
	
	//elimino la memoria condivisa
	printf("sto eliminando la memoria condivisa.......");
	//MEM_CASSA 0x700
	int memCassa = shmget(0x700,0,1);
	if((shmctl(memCassa,IPC_RMID,0))<0)
		perror("rimozione memCassa: ");
	
	printf("fatto \n");
	
	//elimino la coda dei messaggi
	printf("sto eliminando le code dei messaggi.......");
	//CODA_MESS1 0614
	int codaMess1 = msgget(0614,0);
	if((msgctl(codaMess1,IPC_RMID,0))<0)
		perror("rimozione codaMess2: ");
	
	//CODA_MESS2 0615
	int codaMess2 = msgget(0615,0);
	if((msgctl(codaMess2,IPC_RMID,0))<0)
		perror("rimozione codamess2: ");
	
	printf("fatto \n");
	
}


void asciiTitolo(){
	printf("01100001 01110011 01110011 01101001 01100111 01101110 01101101 01100101 01101110 01110100 00000010 \n");
	printf("			 _____                                     \n");
	printf("			|  _  |___ ___ ___ ___ ___ _____ _____ ___ \n");
	printf("			|   __|  _| . | . |  _| .'|     |     | .'|\n");
	printf("			|__|  |_| |___|_  |_| |__,|_|_|_|_|_|_|__,|\n");
	printf("		   		      |___|                        \n");
	
	printf("		 _____                        _ _    _               _      \n");   
	printf("		|  _  |___ _____ ___ ___    _| |_|  | |_ ___ ___ ___|_|___ ___ \n");	
	printf("		|   __| . |     | . | .'|  | . | |  | . | -_|   |- _| |   | .'|\n");
	printf("		|__|  |___|_|_|_|  _|__,|  |___|_|  |___|___|_|_|___|_|_|_|__,|\n");
	printf("		                |_|                                            \n");
	printf("\n 01100011 01101111 01110010 01110000 01101111 01110010 01100001 01110100 01101001 01101111 01101110 \n");
}


void asciiBenzinaio(){
	printf("\n ...CREO IL PROCESSO BENZINAIO... \n");
	 
	printf("		   _____	                   _____  \n");
	printf("		 _[_____]_                       _[_____]_ \n");
	printf("		|---------|                     |---------| \n");
	printf("		|#### ####|\\                    |#### ####|\\ \n");
	printf("		|...   ...|\\\\                   |...   ...|\\\\ \n");
	printf("		|_pompa_1_|((|                  |_pompa_2_|((| \n");
	printf("		|.-------.| ||                  |.-------.| || \n");
	printf("		||       || []                  ||       || [] \n");
	printf("		||       ||  #                  ||       ||  # \n");
	printf("		||       || ,#                  ||       || ,# \n");
	printf("		||       || #'                  ||       || #'\n");
	printf("		||       ||#'                   ||       ||#' \n");
	printf("		||       ||'                    ||       ||'\n");
	printf("	      __|'-------'|__                 __|'-------'|__  \n");
	printf("	     [_______________]               [_______________] \n");

}

void asciiMacchina(){
	printf("\n ...CREO IL PROCESSO MACCHINA... \n");
	
	printf("\n");
	printf("           _________\n");
	printf("          /  ;~~~~~~`\\ \n");
	printf("     ____/   |________\\________ \n");
	printf("    /       :                  `\\ \n");
	printf("   |>   .--.:^^          .--.  _<) \n");
	printf("  {____/ .. \\___________/ .. \\____} \n");
	printf("       \\ '' /           \\ '' / \n");
	printf("        '--'             '--' \n");
	
	return;
}

void asciiCredits(){
	printf("\n");
	
	printf("                       _ _             \n");
	printf("                      | (_)  _         \n");
	printf("  ____  ____ _____  __| |_ _| |_  ___  \n");
	printf(" / ___)/ ___) ___ |/ _  | (_   _)/___) \n");
	printf("( (___| |   | ____( (_| | | | |_|___ | \n");
	printf(" \\____)_|   |_____)\\____|_|  \\__|___/  \n");

	printf("\n");
	
	printf(" _ _  _ _     _ _ . \n");
	printf("_\\(_|| | ||_|| (_|| \n");
	
	printf("\n");
	
	printf(" _ _  _ _  _  _  _.   _ . _  \n");
	printf("(_(_|| | ||_)(_|| |  (_||| | \n");
	printf("          |           _|     \n");

	printf("\n");

	printf("   |. _ _ _  \n");
	printf("|_|||_\\_\\(/_ \n");
    
    printf("\n");
    printf("\n");
    
	printf(" _ _ . _  _  _ |_| / \n");
	printf("_\\| |||_)(/_|    |(_) \n");
    printf("      | \n");             

	printf("\n");
	
	printf(" _. _  _  _  _ _  \n");
	printf("(_||_)|_)(_)_\\(_) \n");
	printf("   |  |           \n");
                   
    printf("\n");
}
