#include   <stdio.h>
#include   <stdlib.h>
#include   <unistd.h>
#include   <string.h>
#include   <ctype.h>
#include   <sys/types.h>
#include   <sys/socket.h>
#include   <netinet/in.h>
#define MAXLENGTH 80
#define SERVER_PORT 1313

int main () {
  int sockfd;
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  int i = 0, len;
  char buf[MAXLENGTH], c;

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
  if (recv(sockfd, buf, 27, 0) > 0) {
      printf("%s", buf);
  } else {
      perror("Connessione al server interrotta");
      exit(3);
  }

  /* acquisizione della stringa da standard input */
  printf("\nScrivi un pathname:");
  while ((c = getchar()) != '\n' && i < MAXLENGTH)
    buf[i++] = c;

  buf[i] = '\0';
  len = strlen(buf);

  printf("\nInvio la stringa al server...\n");
  /* invio e ricezione della stringa */
  if (send(sockfd, buf, len, 0) == -1) {
      perror("Errore nell'invio della stringa al server");
      exit(4);
  }

  if (recv(sockfd, buf, len, 0) > 0) {
      printf("Ho ricevuto la risposta: %s\n", buf);
  } else {
      perror("Connessione al server interrotta");
      exit(3);
  }

  /* chiusura della connessione */
  close(sockfd);
 exit(0);
}
