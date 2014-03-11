#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 1313

int main () {
	//Dati per connessione
	int sockfd;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	int len = 0;
	
	//inizializzo la mia variabile socket (sockfd)
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("chiamata alla system call socket fallita");
		exit(1);
	}

	/* connessione al server */
	if (connect(sockfd, (struct sockaddr *) &server, sizeof server) == -1) {
		perror("connessione al server fallita");
		exit(2);
	}

	/* ricezione e stampa a video del messaggio di benvenuto del server */
	char *buf = NULL;
	if (recv(sockfd, buf, len, 0) > 0) {
		printf("%s", buf);
	} else {
		perror("Connessione al server interrotta");
		exit(3);
	}

	printf("\nInvio al server un pathname\n");
	
	//pathnname da inviare
	long size = pathconf(".", _PC_PATH_MAX);
	char *buf2, *ptr = NULL, *file = "/prova.txt";

	if ((buf2 = (char *)malloc((size_t)size)) != NULL){	//creo un buffer in cui verrà inserito il pathname
		if((ptr = getcwd(buf, (size_t)size)) == NULL){	//recupero il pathname dell'attuale work directory
			printf("\nErrore getcwd()\n");
			free(buf2); //disalloco il buffer
			exit(-1);	
		}
	} else {
		printf("errore allocazione buffer per ricerca pathname ");
		free(buf2);  //disalloco il buffer
		exit(-1);
	}
	free(buf2);	  //disalloco il buffer
	
	puts(ptr); //stampo path trovato... da eliminare in versione definitiva
   
	char *path_completo = strcat (ptr, file); //unisco le due stringhe, pathname directory + nome file
   
   
    puts(path_completo);  //stampo path completo... da eliminare in versione definitiva
    
	/* invio e ricezione della stringa */
	if (send(sockfd, path_completo, size + 10, 0) == -1) {
		perror("Errore nell'invio della stringa al server");
		exit(4);
	}
	
	//Ricezione risposta
	if (recv(sockfd, buf, len, 0) > 0) {
		//stampo a video i vari dati ricevuti
      		printf("Ho ricevuto la risposta: %s\n", buf);
	} else {
		perror("Connessione al server interrotta");
		exit(3);
	}
	
	//chiudo la connessione con il server
	if (send(sockfd, "+FINE+", 6, 0) == -1) {
		perror("Errore nell'invio della stringa al server");
		exit(4);
	}
	
	//Ricezione risposta da server per chiusura connessione
	if (recv(sockfd, buf, len, 0) > 0)
		printf("%s\n", buf); //stampo a video i vari dati ricevuti
	else {
		perror("Connessione al server interrotta");
		exit(3);
	}

	/* chiusura della connessione */
	close(sockfd);
	exit(0);
}
