/* un server per maiuscolare linee di testo */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>

#define SERVER_PORT 1313
#define LINESIZE 10
char *buf = NULL;
	if (recv(sockfd, buf, len, 0) > 0) {
		printf("%s", buf);
	
void cerca (int in, int out) {
  char *inputline;
      if(recv(in, inputline, LINESIZE, 0)>0){
	 printf("%s", buf);
       } else {
		perror("Connessione al server interrotta");
		exit(3);
       }

      printf("\nricevuta stringa %s \n",inputline);
	fopen(inputline,
      send (out, inputline, len, 0);
}


int main (int argc, char **argv) {
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
  
  listen (sock, 5);
  
  
  
  /* gestione delle connessioni dei client */
  while (1) {
      client_len = sizeof (client);
      if ((fd = accept (sock, (struct sockaddr *) &client, &client_len)) < 0) {
	  perror ("accepting connection");
	  exit (3);
      }
      pid_t pid =fork();
      if(pid==0){
      	fprintf (stderr, "Aperta connessione.\n");
      	send (fd, "Benvenuto all'UpperServer!\n", 27, 0);
      	cerca(fd, fd);
      	close (fd);
      	fprintf (stderr, "Chiusa connessione.\n");
      	exit (0);
      }
  }
  exit (0);
}
