//tutte le librerie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

//costanti del server e del client
#define SERVER_PORT 1313
#define SIZEPATH 80
#define SIZEMSG 320
