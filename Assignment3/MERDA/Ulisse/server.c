#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h> 

#define SERVER_PORT 1313
#define LINESIZE 80

int main(){
	int sock, fd;
	socklen_t client_len;
	struct sockaddr_in server, client;

	/* impostazione del transport end point */
	if ((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
		perror ("chiamata alla system call socket fallita");
		exit (1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons (SERVER_PORT);

	/* binding dell'indirizzo al transport end point */
	if (bind (sock, (struct sockaddr *) &server, sizeof server) == -1) {
		perror ("chiamata alla system call bind fallita");
		exit (2);
	}

	//bisogna gestire il fatto che operi con più client personalmente
	listen (sock, 1);

	/* gestione delle connessioni dei client */
	while (1) {
		client_len = sizeof (client);
		if ((fd = accept (sock, (struct sockaddr *) &client, &client_len)) < 0) {
		perror ("accepting connection");
		exit (3);
	}
	fprintf (stderr, "Aperta connessione.\n");
    
    //lavoro del server
    //Ricevo il pathname
    char inputline[LINESIZE];
    if ((recv(fd, inputline, LINESIZE, 0)) > 0) {
		printf("Ho ricevuto il pathname: %s\n", inputline);
	} else {
		perror("Connessione al server interrotta");
		exit(4);
	}
	
	//verifica se localmente al server esiste un file identificato da quel pathname
	
	//se esiste invio i dati
	//pensavo di fare un array con più dati divisi da un qualche carattere 
	/*
		da slide: File and Dir 2x2: stat è una struttura definita in <sys/stat.h> 
		che comprende i seguenti componenti (i tipi sono definiti in <sys/types.h>):
		uid, tempo dell'ultima modifica, mi sembra anche dimensione ma nn ho visto la data di creazione
	*/
	
	//send (fd, **dati file**, lenght, 0);  //dimensione, data di creazione, di ultima modifica, uid dell'utente proprietario.
	
	
	char *fine = "+FINE+";
	if ((recv(fd, inputline, LINESIZE, 0)) > 0) {
		if(strcmp(inputline, fine) == 0) {
			if (send(fd, "CIAO", 4, 0) == -1) {
				perror("Errore nell'invio della stringa al server");
				exit(5);
			}
			//termino
		}
	} else {
		perror("Connessione al server interrotta");
		exit(6);
	}
    
    
    close (fd);
    fprintf (stderr, "Chiusa connessione.\n");
  }
  exit (0);
}
