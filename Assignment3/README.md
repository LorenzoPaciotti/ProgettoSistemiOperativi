#ASSIGNMENT III

###Testo dell’esercizio:
Si realizzi in C il codice relativo ai processi server e client che operino in questo modo:
Il processo server fornisce un servizio "informazioni" sui file presenti nel "suo" filesystem.
In pratica il server opera cosi':
accetta connessione da un generico client.
il client invia al server un pathname. Il Server verifica se localmente al server esiste un file identificato da quel pathname. Se il file non esiste comunica al client che il fine non c'e'. 
Se il file esiste, restituisce al client delle statistiche sul file, ovvero:
dimensione, data di creazione, di ulitma modifica, uid dell'utente proprietario.

Tutte le trasmissioni devono avvenire tramite socket.
Il server deve poter servire contemporaneamente piu' client.
Il server termina quando un client gli spedisce la stringa "+FINE+" (in tal caso, prima di terminare restituisce al client la stringa "CIAO").

In caso di aspetti non specificati nel testo dell'esercizio o in ogni caso in cui si possano
configurare diverse scelte alternative nella progettazione della soluzione, effettuate voi
le scelte che vi sembrano piu` ragionevoli (giustificandole opportunamente nei commenti al codice). 

ALTRE INDICAZIONI GENERALI:
- realizzare il progetto in C in ambiente UNIX/Linux;
- il codice deve essere indentato in modo chiaro ed debitamente commentato;
- si richiede una adeguata gestione degli errori e delle anomalie (es. fallimento nella chiamata ad una system call), possibilmente con una messaggistica di errore che informi l'utente su eventuali anomalie accadute a tempo di esecuzione; (Possibilmente: se l'errore impone di terminare l'esecuzione, si devono prima  disallocare le risorse allocate.)
- il progetto deve essere corredato da un Makefile che permetta la compilazione e la generazione del/degli eseguibile/i;
- si eviti la generazione di processi zombie e il deadlock;
- per tutti i processi che terminano e tutte le risorse e le strutture di comunicazione utilizzare dai processi devono essere de-allocate; Nessun processo deve essere ucciso volontariamente da un altro processo, ma deve sempre terminare normalmente la sua esecuzione;

###Relazione
####Note generali e scelte di progetto
Per affrontare il problema ho eseguito una strategia bottom-up, creando prima il sistema di connessione client/server attraverso i socket. In seguito la gestione di più client contemporaneamente, il sistema di comunicazione e infine la gestione dei segnali per la chiusura pulita di tutte le istanze del server.
Per semplificare tutto il progetto ho implementato la libreria libClientServer.h, con all’interno le costanti che servono al client e al server e le varie librerie esterne.

####Fork e Socket
Per la gestione di più client contemporaneamente ho deciso di utilizzare le fork. Il server crea il socket attraverso le system call socket() e bind(). Resta in ascolto sulla porta tramite la system call listen(). Ogni volta che il client tenta una connessione su questa porta, il server attraverso la system call accept() riceve l’identificatore del socket del client. A questo punto il server fa una fork e il figlio inizia la comunicazione con il client. Il server padre ritorna a eseguire la accept() aspettando nuovi client e ripetendo le stesse operazioni in modo tale da permettere la comunicazione con più client.                                             

####Sistema di comunicazione
Una volta creato il canale di comunicazione, attraverso il socket, di un server figlio con un client avviene la comunicazione vera e propria attraverso le system call send() e la recv().
Come proponeva l’esercizio, il client invia un pathname e il server risponde con le specifiche del file se esiste e in caso contrario con la stringa ”il file non esiste”. Per le comunicazioni vengono usati dei buffer di stringhe di lunghezza definita all’interno della libreria.
Per trovare le informazioni del file o della cartella il server figlio esegue la system call stat(), se questa restituisce -1 e l’errore è ENOENT il file non esiste, mentre se esiste tutte le proprietà vengono salvate in una struttura. A questo punto il server crea una stringa formattata già nel modo corretto con tutte le informazioni e la invia. Il client la riceve e la stampa direttamente a schermo.

####Segnali
Per la terminazione pulita del server i processi hanno bisogno di due segnali, il primo gestito dalla funzione terminaPadre() e il secondo dalla funzione terminaServer(). Per mandare i due diversi segnali ho assegnato al padre e ai figli gruppi diversi ed usato la system call killpg() in grado di inviarli a un intero gruppo.
In questo modo posso inviare il segnale 30 solo al padre e il 31 ai figli server.

####Chiusura della connessione
La comunicazione client e server figlio termina quando il client invia il carattere 'q', in questo modo il client chiude il socket, il che provoca una errore nella recv() del server, che esce dal ciclo e chiude la connessione.
Se invece il client invia al Server figlio la stringa “+FINE+”,  avviene una chiusura pulita di tutto il server. In questo caso il server figlio che riceve questa stringa invia i segnali al padre e agli altri figli. Nella funzione terminaPadre che viene così eseguita il padre si mette in wait() aspettando la terminazione dei figli, mentre nella procedura terminaServer() i figli chiudono la connessione con il client e terminano.

###Strutture
**sockaddr_in**
è una struttura che contiene l’indirizzo su cui il socket si mette in ascolto. Nel nostro assignment abbiamo utilizzato i parametri standard.

	struct sockaddr_in {
	    short            sin_family;   // e.g. AF_INET
	    unsigned short   sin_port;     // e.g. htons(3490)
	    struct in_addr   sin_addr;     // see struct in_addr, below
	}

+ *sin_family:* va sempre impostata AF_INET.
+ *sin_port:* un numero di quattro cifre che identifica la porta di connessione, per fare questo si utilizza la funzione htons(int port), che restituisce il valore in formato byte.
+ *sin_addr:* è un puntatore a una struttura che contiene l’indirizzo vero e proprio.

**in_addr**
questa struttura contiene l’indirizzo della rete
	
	struct in_addr {
	    uint32_t       s_addr;     /* address in network byte order */
	}
	
+ *s_addr:* per scrivere l’indirizzo si utilizza la funzione htonl(int) che lo converte in formato byte, nel nostro caso abbiamo utilizzato la costante INADDR_ANY, che indica il primo indirizzo disponibile libero.

**sigaction**
Questa struttura contiene le indicazioni associate ad un dato segnale
	
	struct sigaction {
	   void     (*sa_handler)(int);
	    void     (*sa_sigaction)(int, siginfo_t *, void *);
	    sigset_t   sa_mask;
	    int        sa_flags;
	    void     (*sa_restorer)(void);
	}
	
+ *sa_handle:* indica la funzione eseguita all’arrivo del segnale

**stat**
E’ una struttura che viene passata per parametro alla funzione stat
	
	struct stat {
	    dev_t     st_dev;     /* ID of device containing file */
	    ino_t     st_ino;     /* inode number */
	    mode_t    st_mode;    /* protection */
	    nlink_t   st_nlink;   /* number of hard links */
	    uid_t     st_uid;     /* user ID of owner */
	    gid_t     st_gid;     /* group ID of owner */
	    dev_t     st_rdev;    /* device ID (if special file) */
	    off_t     st_size;    /* total size, in bytes */
	    blksize_t st_blksize; /* blocksize for file system I/O */
	    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
	    time_t    st_atime;   /* time of last access */
	    time_t    st_mtime;   /* time of last modification */
	    time_t    st_ctime;   /* time of last status change */
	}
	
Il tipo time_t è un intero che indica i secondi dal 1970 al momento della chiamata, per avere una formattazione standard di data si usa la funzione ctime che restituisce una stringa contenente la data

###System Call
**socket (lato client/server)**
crea un socket e ritorna un parametro identificativo di questo
+ *Prototipo:* sockfd socket(int domain, int type, int protocol)
+ *domain:* specifica un protocollo di connessione, nel nostro caso abbiamo utilizzato IPv4, attraverso la costante IAF_INET
+ *type:* indica il tipo di comunicazione che si vuole usare nel socket, nel nostro caso abbiamo utilizzato SOCK_STREAM, che indica una comunicazioni bilaterale attraverso un flusso di byte
+ *protocol:* è un protocollo, di solito impostato a 0
+ *valore di ritorno:* identificativo del socket

**bind (lato server)**
assegna al socket l’indirizzo che poi verrà utilizzato dal client per la connessione.
+ *Prototipo:* int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
+ *addr:* un puntatore alla struttura che identifica l’indirizzo da dare al socket
+ *addrlen:* la lunghezza in byte della struttura a cui punta addr.

**listen (lato server)**
Il socket appena creato è marcato passivo, attraverso questa s.c. può essere impostato per accettare richieste di connessione esterne
+ *prototipo:* int listen(int sockfd, int backlog)
+ *backlog:* la grandezza della coda di attesa per la connessione al server, se arriva una connessione mentre la coda è + piena restituisce un messaggio di errore al client. Nel nostro caso è stata impostata a 1.

**accept (lato server)**
questa s.c. prende la prima richiesta dalla coda delle connessioni in entrata, instaura una connessione e restituisce il descrittore del socket a cui si è connesso.
+ *prototipo:* int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

**connect (lato client)**
attraverso questa s.c. si connette il socket del client a quello del server, e si richiede così una nuova connessione.
+ *prototipo:* int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
+ *sockfd:* il socket del client che prova la connessione
+ *addr:* indirizo del server a cui ci si vuole connettere

**send/recv**
permette di inviare e ricevere messaggi tramite socket, questo è possibile solo se c’è una connessione attiva.
+ *prototipo:* ssize_t send(int sockfd, const void *buf, size_t len, int flags)
ssize_t recv(int sockfd, void *buf, size_t len, int flags)
+ *buf:* il messaggio che si vuole inviare
+ *len:* lunghezza del messaggio

**close**
chiude la connessione fra due socket, questa può essere fatta sia lato client che lato server.
+ *prototipo:* int close(int sockfd)

**sigaction**
modifica l’azione compiuta da un processo quando gli arriva uno specifico segnale
+ *prototipo:* int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) 
+ *signum:* la costante che identifica il segnale al quale gli si vuole applicare un nuovo comportamento, non sono ammessi i signali sigkill e sigstop, i quali non possono essere rieditati.
+ *act:* è un puntatore a una struttura sigaction in cui viene inserito il nome della funzione da eseguire all’arrivo del segnale.
+ *oldact:* qui viene memorizzata la vecchia struttura del segnale, se non si vuole salvare il precedente stato del segnale si può lasciare a NULL

**kill**
Attraverso questa s.c. è possibile inviare un qualsiasi segnale a uno specifico processo.
+ *prototipo:* int kill(pid_t pid, int sig)
+ *pid:* id del processo a cui mandare il segnale, se questo è uguale a 0 il segnale viene mandato a tutti i processi del suo gruppo, se invece è uguale a -1 lo invia a tutti i processi al quale ha i permessi di inviarlo, tranne a init.
+ *sig:* la costante che identifica il segnale che si vuole inviare.

**killpg**
questa s.c. invia lo specifico segnale ad un intero gruppo di processi.
+ *prototipo:* int killpg(int pgrp, int sig) 
+ *pgrp:* identificatore del gruppo di processi al quale si vuole inviare il segnale.

**getpgid**
ritorna l’identificatore del gruppo di processi
+ *prototipo:* pid_t getpgid(pid_t pid)

**setpgid**
Modifica id del gruppo di processi.
+ *prototipo:* int setpgid(pid_t pid, pid_t pgid)
+ *pid:* indica il processo a cui vai a cambiare il pgid. Se questo è 0 significa che lo cambia a se stesso
+ *pgid:* indica il gruppo da associare al pid, se è 0 il pgid diventa uguale al pid di chi chiama questa s.c.

**stat**
Ritorna le proprietà del file passato per parametro
+ *prototipo:* int stat(const char *path, struct stat *buf)
+ *path:* pathname del file
+ *buf:* è un puntatore ad una struttura in cui scriverà le varie proprietà
+ *errori:* ENONET indica che il file non esiste

###Funzioni di supporto
**sprintf**
questa funzione fa la stessa cosa di printf, con la differenza che non visualizza il contenuto su lo standar output, ma lo inserisce all’interno di una stringa, passata per puntatore
+ *prototipo:* int sprintf(char *str, const char *format, ...)

**strcmp**
confronta due stringhe restituendo un valore minore di zero se la stringa s1 è più piccola di s2, uguale a zero se sono uguali e maggiore di zero se s1 è più grande di s2, in ordine lessicografico..
+ *prototipo:* int strcmp(const char *s1, const char *s2)

**strcpy**
questa funziona copia la stringa src nella stringa dest, naturalmente la stringa di destinazione deve essere maggiore o uguale a quella da copiare 
+ *prototipo:* char *strcpy(char *dest, const char *src)

####Makefile
	progetto: src/libClientServer.o client server
	
	src/libClientServer.o: src/libClientServer.c src/libClientServer.h
		cd src && gcc -g -c libClientServer.c
	
	client: src/client.c src/libClientServer.o
		cd src && gcc -g client.c libClientServer.o -o ../client
	
	server: src/server.c src/libClientServer.o
		cd src && gcc -g server.c libClientServer.o -o ../server
