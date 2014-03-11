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

/* CODA MESSSAGGI */
typedef struct my_msg {
	long mtype;
	int mtext;
}my_msg;

int scrivi_msg(int coda,my_msg *a,int importo){
	int rispostamsg;
		a->mtype=1;
		a->mtext=importo;
		if( (rispostamsg = msgsnd(coda,a,sizeof(int),0))<0){
			perror("msgsnd scrivi msg: ");
			return(-1);
		}
	return(0);			
}

int ricevi_msg(int coda,my_msg *a){
	int mlen;
		if ( (mlen = msgrcv(coda, a,sizeof(int),0, MSG_NOERROR)<0)){
			perror("msgrcv ricevi messaggio: ");
			return(-1);
		}
	return(0);
}

int  crea_coda( key_t key ){
	int msg_qid;
	msg_qid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
	if (msg_qid < 0){
		if (errno == EEXIST) {
			 msg_qid = msgget(key, 0);
			 if(msg_qid<0)
				perror("crea coda: ");
			 }
		else{
			printf("errore crea coda msg fallita\n");
			return(-1);
			}
	} 
	return msg_qid;
}


/* SEMAFORI */
union _semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array; } semun;

int waitSem(int semid) {
	struct sembuf wait_buf;
	wait_buf.sem_num = 0;
	wait_buf.sem_op = -1;
	wait_buf.sem_flg = 0;
	if((semop(semid, &wait_buf, 1)) <0) {
		perror("waitSem fallita");
		return(-1);
	}
	return(0);
}

int signalSem(int semid) {
	struct sembuf signal_buf;
	signal_buf.sem_num = 0;
	signal_buf.sem_op = 1;
	signal_buf.sem_flg = 0;
	if(semop(semid, &signal_buf, 1) <0) {
		perror("signalSem fallita");
		return(-1);
	}
	return(0);
}


/* ASCII */
void asciiTitolo(){
	printf("\n01100001 01110011 01110011 01101001 01100111 01101110 01101101 01100101 01101110 01110100 00000010 \n");
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
	printf("\n01100011 01101111 01110010 01110000 01101111 01110010 01100001 01110100 01101001 01101111 01101110 \n");
}

void asciiSem_mem(){
	printf("\n...CREO IL PROCESSO SEM_MEM...\n");
	printf("\n");
    printf("              _[]_\n");
    printf("             [____]\n");
    printf("         .----'  '----.\n");
    printf("     .===|    .==.    |===.\n");
    printf("     \\   |   /####\\   |   /\n");
    printf("     /   |   \\####/   |   \\\n");
    printf("     '===|    `\"\"`    |==='\n");
    printf("     .===|    .==.    |===.\n");
    printf("     \\   |   /::::\\   |   /\n");
    printf("     /   |   \\::::/   |   \\\n");
    printf("     '===|    `\"\"`    |==='\n");
    printf("     .===|    .==.    |===.\n");
    printf("     \\   |   /&&&&\\   |   /\n");
    printf("     /   |   \\&&&&/   |   \\\n");
    printf("     '===|    `\"\"`    |==='\n");
	printf("         '--.______.--'	\n");
	printf("\n");
	
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

	printf("\n01100001 01110011 01110011 01101001 01100111 01101110 01101101 01100101 01101110 01110100 00000010 \n");

	printf("                       _ _             \n");
	printf("                      | (_)  _         \n");
	printf("  ____  ____ _____  __| |_ _| |_  ___  \n");
	printf(" / ___)/ ___) ___ |/ _  | (_   _)/___) \n");
	printf("( (___| |   | ____( (_| | | | |_|___ | \n");
	printf(" \\____)_|   |_____)\\____|_|  \\__|___/  \n");

	printf("\n");
	
	printf("\t _ _  _ _     _ _ . \n");
	printf("\t_\\(_|| | ||_|| (_|| \n");
	
	printf("\n");
	
	printf("\t\t _ _  _ _  _  _  _.   _ . _  \n");
	printf("\t\t(_(_|| | ||_)(_|| |  (_||| | \n");
	printf("\t\t          |           _|     \n");

	printf("\n");

	printf("\t\t\t   |. _ _ _  \n");
	printf("\t\t\t|_|||_\\_\\(/_ \n");
    
    printf("\n");
    printf("\n");
    
	printf("\t\t\t\t _ _ . _  _  _ |_| / \n");
	printf("\t\t\t\t_\\| |||_)(/_|    |(_) \n");
    printf("\t\t\t\t      | \n");             

	printf("\n");
	
	printf("\t\t\t\t\t _. _  _  _  _ _  \n");
	printf("\t\t\t\t\t(_||_)|_)(_)_\\(_) \n");
	printf("\t\t\t\t\t   |  |           \n");
               
	printf("\n01100011 01101111 01110010 01110000 01101111 01110010 01100001 01110100 01101001 01101111 01101110 \n");    
}
