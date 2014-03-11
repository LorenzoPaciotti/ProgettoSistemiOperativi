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
#include <limits.h>

#define SEMPERM 0600
#define SEM_STAZIONE 0601
#define SEM_POMPA1 0602
#define SEM_POMPA2 0603
#define SEM_PIENOFATTO1 0604
#define SEM_PIENOFATTO2 0605
#define SEM_SOLDIINVIATI1 0606
#define SEM_SOLDIINVIATI2 0607
#define SEM_VATTENE1 0610
#define SEM_VATTENE2 0611
#define SEM_MUTEXCASSA 0612
#define SEM_POMPA1LIBERA 0613
#define MEM_CONDIVISA 0700
#define SEM_NUMEROMACCHINE 0616
#define	CODA_MESS1 0614
#define CODA_MESS2 0615
#define NUMERO_MACCHINE 5


typedef struct my_msg {
	long mtype;
	int mtext;
}my_msg;

void scrivi_msg(int coda, my_msg *a, int importo);
void ricevi_msg(int coda, my_msg *a);
int  crea_coda( key_t key );

//~ SEMAFORI

typedef union _semun {
	int val;
	struct semid_ds *buf;
	ushort *array; } semun;
	
int waitSem(int semid);
int signalSem(int semid);

void asciiBenzinaio();
void asciiMacchina();
