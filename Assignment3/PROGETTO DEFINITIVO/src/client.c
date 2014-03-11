#include "libClientServer.h"

int main () {
	int sockfd;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	int i = 0, len;
	char buf[SIZEPATH], c;
	char buffer[SIZEMSG];
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("chiamata alla system call socket fallita");
		exit(-1);
	}

	//connessione al server
	if (connect(sockfd, (struct sockaddr *) &server, sizeof server) == -1) {
		perror("connessione al server fallita");
		exit(-1);
	}

	//ricezione e stampa a video del messaggio di benvenuto del server
	if (recv(sockfd, buf, 27, 0) > 0) {
		printf("%s", buf);
	} else {
		perror("Connessione al server interrotta");
		exit(-1);
	}
	
	while(strcmp(buf,"+FINE+") != 0){
		printf("\nDigita il path da cercare, q per terminare la singola connessione, '+FINE+' per terminare il server: ");
		i=0;
		//aquisisco la stringa dal terminale
		while (((c = getchar()) != '\n')&& i < SIZEPATH)
			buf[i++] = c;
		buf[i++]='\0';
		len = i+1;
		//se la stringa inserita è uguale a q il client chiude la connessione
		if(strcmp(buf,"q\0") == 0)
			break;
		
		printf("Invio la stringa contenente il path al server...\n");
		//invio la stringa al server
		if (send(sockfd, buf, len, 0) < 0) {
			perror("Errore nell'invio della stringa al server");
			exit(-1);
		}
		
		
		if (recv(sockfd,buffer,sizeof(buffer), 0) > 0) {
			printf("\nRisposta Server:\n%s",buffer);
		} else {
			perror("Connessione al server interrotta");
			exit(-1);
		}
		strcpy(buffer, "");
	}
	//chiusura della connessione
	close(sockfd);
	exit(0);
}
