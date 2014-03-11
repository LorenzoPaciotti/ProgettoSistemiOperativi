/* un server per maiuscolare linee di testo */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define SERVER_PORT 1313
#define LINESIZE 80

void upperlines (int in, int out) {
  char inputline[LINESIZE];
  int len, i;

  while ((len = recv (in, inputline, LINESIZE, 0)) > 0) {
      printf("\nHo ricevuto la stringa, ora la converto...\n");
      sleep(5);
      for (i = 0; i < len; i++)
	inputline[i] = toupper (inputline[i]);
      printf("\n...e la rispedisco...\n");
      send (out, inputline, len, 0);
  }
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

  listen (sock, 1);

  /* gestione delle connessioni dei client */
  while (1) {
      client_len = sizeof (client);
      if ((fd = accept (sock, (struct sockaddr *) &client, &client_len)) < 0) {
	  perror ("accepting connection");
	  exit (3);
      }
      fprintf (stderr, "Aperta connessione.\n");
      send (fd, "Benvenuto all'UpperServer!\n", 27, 0);
      upperlines (fd, fd);
      close (fd);
      fprintf (stderr, "Chiusa connessione.\n");
  }
  exit (0);
}
