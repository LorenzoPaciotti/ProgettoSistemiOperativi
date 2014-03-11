#include "libClientServer.h"

int fdGlob;
int numeroFigliFatti=0;


void terminaServer(int signo);
void terminaPadre();
void lavoro (int in, int out);

int main (int argc, char **argv) {
	int sock, fd;
	socklen_t client_len;
	struct sockaddr_in server, client;
	int pgid_figli=0;
	int creareGruppo;
	printf("Avvio del Server\n");
	
	//alloca 2 strutture sigaction per la gestione dei segnali del padre e dei figli 
	struct sigaction *sigFigli = malloc(sizeof(struct sigaction));
	sigFigli -> sa_handler = terminaServer;
	struct sigaction *sigPadre = malloc(sizeof(struct sigaction));
	sigPadre -> sa_handler = terminaPadre;
	
	//attiva gli handler per la gestione dei segnali 30 e 31
	sigaction(30,sigFigli,sigFigli);
	sigaction(31,sigPadre,sigPadre);
	
	//impostazione del socket
	if ((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
		perror ("chiamata alla system call socket fallita");
		exit (-1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons (SERVER_PORT);

	//binding dell'indirizzo al transport end point
	if (bind (sock, (struct sockaddr *) &server, sizeof server) == -1) {
		perror ("chiamata alla system call bind fallita");
		exit (-1);
	}
	listen (sock, 1);
	
	creareGruppo = 1;
	
	//gestione delle connessioni dei client
	while (1) {
		client_len = sizeof (client);
		if ((fd = accept (sock, (struct sockaddr *) &client, &client_len)) < 0) {
			perror ("accept:");
			exit (-1);
		}
		
		//creo un figlio del server per fargli gestire la connessione con il client
		pid_t pidFiglio=fork();
		if(pidFiglio==0){
			printf ("Il figlio (pid %d) ha aperto la connessione\n", getpid());
			send (fd, "Benvenuto!\n", 13, 0);
			//esegue il suo lavoro e poi chiude la connessione
			lavoro (fd, fd);
			close (fd);
			printf ("Il figlio (pid %d) ha chiuso la connessione\n", getpid());
			exit(0);
		}else{
			
			//il padre cambia il gruppo dei processi figli in modo ma poter mandare due signeli distinti
			if(creareGruppo == 1){
				setpgid(pidFiglio,0);
				pgid_figli = getpgid(pidFiglio);
				creareGruppo = 0;
			}
			else
				setpgid(pidFiglio,pgid_figli);
			numeroFigliFatti++;
		}
	}
	exit (0);
}

void terminaServer(int signo){
	printf("sono in terminaServer e il mio pid è %d \n",getpid());
	close(fdGlob);
	exit(0);
}

//funzione che gestisce l'handler del segnale 31, per il padre
void terminaPadre(){
	printf("sono in terminaPadre, mi metto in wait e il mio pid è %d \n",getpid());
	//il padre aspetta la terminazione dei figli
	while(numeroFigliFatti > 0){
		wait(NULL);
		numeroFigliFatti--;
	}
	exit(0);
}

void lavoro (int in, int out) { 
	fdGlob = in;
	char inputline[SIZEPATH];
	int len;
	struct stat *file_stats=malloc(sizeof(struct stat)); // alloco la struttura di tipo stat dove andra a scrivere le info
	char msg[SIZEMSG];									//  la funzione stat 


	while ((len = recv (in, inputline, SIZEPATH, 0)) > 0) {
		//se il client scrive la stringa +FINE+ devo chiudere tutto il server
		if(strcmp(inputline,"+FINE+")==0){
			printf("devo terminare\n");
			send(out, "CIAO!\n", 8, 0);
			//mando il segnale 31 al padre
			kill(getppid(), 31);
			//mando il segnale 30 a tutto il mio gruppo
			killpg(getpgid(getpid()), 30);
		}else{
			printf("Ho ricevuto il seguente path:  %s \n",inputline);
			//utilizzo la funzione stat per accedere alle proprietà del file
			if((stat(inputline, file_stats)) < 0) {
				//verifico se l'errore avenuto è perchè il file non esiste
				if(errno==ENOENT){
					send (out, "Il file non esiste\n", 20, 0);					
				}else
					perror("fstat");
			}
			else{	
				//creao il messaggio con tutte le informazioni del file richiesto e lo invio
				char string1[80];
				char string2[80];
				char string3[80];
				char string4[80];
				sprintf (string1, "user ID of owner: %d\n", file_stats->st_uid);
				sprintf (string2, "total size, in bytes: %ld\n", file_stats->st_size);
				sprintf (string3, "time of last modification: %s", ctime(&file_stats->st_mtime));
				sprintf (string4, "time of last change: %s", ctime(&file_stats->st_ctime));
				sprintf (msg, "%s%s%s%s", string1,string2,string3,string4);
				send (out, msg, sizeof(msg), 0);
			}
		}
	}
}
