#ASSIGNMENT II

###Testo dell’esercizio:
Si realizzi in C una soluzione della seguente versione del problema dei "due benzinai".
Ad una stazione di rifornimento ci sono due pompe di benzina. Vi sono due benzinai che possono: aspettare le auto o operare ad una delle pompe.
Ogni benzinaio lavora sempre alla stessa pompa.
Le auto giungono alla stazione, se vi sono pompe libere si dirigono verso una di esse e si fermano ed il benzinaio addetto a quella pompa riempie il serbatoio dell'auto.
Finito il rifornimento l'auto chiede il costo del pieno. Il benzinaio comunica l'ammontare della spesa. L'auto paga e il benzinaio prende i soldi.
Fatto ciò l'auto riparte. Il benzinaio porta i soldi alla cassa (che è unica), li deposita e torna ad aspettare la prossima auto.
Se non vi sono pompe libere l'auto aspetta che una pompa si liberi.
Rifornita l'ultima auto i due benzinai devono dividersi l'incasso della giornata e "chiudere" la stazione.

Risolvere il problema utilizzando i semafori per la sincronizzazione e la mutua esclusione.
La comunicazione tra benzinaio e auto deve avvenire utilizzando o delle code di messaggi o delle memorie condivise (o entrambe, se preferite).

Prevedere che vi sia un numero prefissato K di auto (specificato da una costante nel programma, oppure da un argomento passato sulla linea del comando, oppure...).

In caso di aspetti non specificati nel testo dell'esercizio o in ogni caso in cui si possano configurare diverse scelte alternative nella progettazione della soluzione, effettuate voi le scelte che vi sembrano piu` ragionevoli (giustificandole opportunamente nei commenti al codice). 

ALTRE INDICAZIONI GENERALI:
- realizzare il progetto in C in ambiente UNIX/Linux;
- il codice deve essere indentato in modo chiaro ed debitamente commentato;
- si richiede una adeguata gestione degli errori e delle anomalie (es. fallimento nella chiamata ad una system call), possibilmente con una messaggistica di errore che informi l'utente su eventuali anomalie accadute a tempo di esecuzione; (Possibilmente: se l'errore impone di terminare l'esecuzione, si devono prima disallocare le risorse allocate.)
- il progetto deve essere corredato da un Makefile che permetta la compilazione e la generazione del/degli eseguibile/i;
- si eviti la generazione di processi zombie e il deadlock;
- per tutti i processi che terminano e tutte le risorse e le strutture di comunicazione utilizzare dai processi devono essere de-allocate; Nessun processo deve essere ucciso volontariamente da un altro processo, ma deve sempre terminare normalmente la sua esecuzione;



###Relazione
####Note generali e scelte di progetto
Per affrontare il problema ho scelto di intraprendere una strategia mista bottom-up/top-down, a partire dalla scrittura della pseudocodifica che dava una visione generale della soluzione del problema, la prima azione è stata il suddividere la struttura in moduli principali. 
In particolare il file 'benzinaio.c' con il codice eseguito dai benzinai, e il file 'macchina.c' con il codice delle auto.
Per la gestione della comunicazione e della sincronizzazione utilizzo i semafori, le code di messaggi e la memoria condivisa, attraverso il file 'sem_mem.c'. Questo gestisce la creazione e l'inizializzazione di queste strutture.

Una volta terminata la parte relativa alla gestione del comportamento di benzinai e auto, ho creato il file 'starter.c' che si occupa di eseguire gli altri file in modo sequenziale. Per risolvere questo problema ho deciso di creare un unico eseguibile che genera tre figli e gli fa eseguire i vari codici, gestendo i possibili errori. 

Per migliorare la compattezza del codice ho deciso di implementare la libreria con il file 'tutto.h' e 'tutto.c'. Questo mi permette di definire una sola volta le funzioni che servono a tutti gli eseguibili, le costanti delle chiavi per le strutture dati e includere le librerie di sistema indispensabili per l'esecuzione del programma.

Infine ho creato il makefile per compilare tutti i file '.h' e '.c' nel modo corretto. In particolare questo prima crea la cartella 'bin', poi crea il file oggetto '.o' del file 'tutto.c' che deve essere linkato con tutti gli altri quattro eseguibili, evitando di ricompilarlo più volte. 
Dopo di che crea gli eseguibili 'sem_mem' 'benzinaio' 'macchina' e li mette nella cartella 'bin', mentre il file 'starter' viene messo nella stessa cartella del makefile, per facilitare l'avvio.

####Fork e gestione dei processi
Per quanto riguarda la ramificazione nella creazioni dei processi, inizialmente il processo 'starter' crea un figlio che esegue il codice 'sem_mem' per l'inizializzazione delle strutture. Se tutto va a buon fine viene creato un altro figlio che fa eseguire il codice 'benzinaio' e un terzo figlio per il codice 'auto'.
A sua volta il processo benzinaio crea un figlio ed entrambi andranno a gestire una pompa. Alla fine del suo codice il padre aspetta la terminazione del figlio gestendo i suoi errori.
Per quanto riguarda il processo auto questo crea n figli impostabili attraverso una costante nel file 'tutto.h', il padre di questi processi non diventa una macchina, ma aspetta la terminazione di tutte le auto figlie gestendo gli eventuali errori di ritorno di una di queste.
Il processo starter aspetta la terminazione del processo benzinaio e di quello auto, gestisce i loro errori e poi rimuove tutte le strutture dati e termina.


###Pseudocodice
	memoria condivisa:
	boolean pompa1 = TRUE
	int numeroAuto = N
	boolean finegiornata = FALSE
	int cassa = 0
	
	semafori:
	SEM_STAZIONE = 2
	SEM_POMPA1 = 0
	SEM_POMPA2 = 0
	SEM_PIENOFATTO1 = 0
	SEM_PIENOFATTO2 = 0
	SEM_SOLDIINVIATI1 = 0
	SEM_SOLDIINVIATI2 = 0
	SEM_VATTENE1 = 0
	SEM_VATTENE2 = 0
	SEM_MUTEXCASSA = 1
	SEM_POMPA1LIBERA = 1
	SEM_NUMEROMACCHINE = 1
	
	code:
	CODA_MESS1
	CODA_MESS2 
	
	Macchine{
		wait(SEM_STAZIONE)
		wait(SEM_POMPA1LIBERA)
		if(pompa1 == TRUE){
			pompa1 = FALSE
			p=1
		}
		signal(SEM_POMPA1LIBERA)
	
		if(p == 1){
			signal(SEM_POMPA1)
			---- ricevo benzina ----
			wait(SEM_PIENOFATTO1)
			---- ricevo importo da pagare ----
			---- invio soldi da pagare ----
			signal(SEM_SOLDIINVIATI1)
			wait(SEM_VATTENE1)
			wait(SEM_POMPA1LIBERA)
			pompa1 = TRUE
			signal(SEM_POMPA1LIBERA)
		}else{	
			signal(SEM_POMPA2)
			---- ricevo benzina ----
			wait(SEM_PIENOFATTO2)
			---- ricevo importo da pagare ----
			---- invio soldi da pagare ----
			signal(SEM_SOLDIINVIATI2)
			wait(SEM_VATTENE2)
		}
	}
	
	Benzinaio{
		wait(SEM_NUMEROMACCHINE)
	
		while( numeroAuto > 0){
			signal(SEM_NUMEROMACCHINE)
			wait(idSemPompa_i)
			
			if(fineGiornata == TRUE){
				break
			}else{
				---- metto benzina ----
				---- invio importo da pagare ----
				signal(idSemPienoFatto_i)
				wait(idSemSoldiInviati_i)
				soldi = ---- ricevo soldi ----
				signal(idSemVattene_i)
				
				wait(SEM_MUTEXCASSA)
				cassa = cassa + soldi
				signal(SEM_MUTEXCASSA)
			
				wait(SEM_NUMEROMACCHINE)
				numeroAuto  =  numeroAuto -1
			}
		}
		signal(SEM_NUMEROMACCHINE)
		signal(idSemPompa_j)
		fineGiornata = TRUE
	
		wait(SEM_MUTEXCASSA)
		paga = cassa / 2
		signal(SEM_MUTEXCASSA)
	}

###Struttura Specifica 
La struttura dell'applicativo consta di alcuni file eseguibili che sono preposti a gestire alcuni aspetti specifici del problema.
+ *MAKEFILE*: contiene le specifiche per la compilazione dei sottostanti file.
+ *STARTER.c*: gestisce l'avvio e la terminazione di tutti i processi che compongono il programma
+ *TUTTO.c*: contiene le funzioni utili a tutti i processi.
+ *TUTTO.h*: contiene i prototipi delle funzioni comuni, le costanti e le altre librerie.
+ *SEM_MEM.c*: crea e inizializza i semafori, la memoria condivisa e le code dei messaggi. 
+ *MACCHINA.c*: crea i processi macchina e contiene il relativo codice dei processi.
+ *BENZINAIO.c*: crea i processi benzinaio e contiene il relativo codice dei processi.

###Strutture:
**sembuf**
questa struttura è utilizzata dalla s.c. semop,all’interno ci sono le impostazioni del semaforo da modificare.
	struct sembuf {
		unsigned short sem_num;  /* semaphore number */
		short          sem_op;   /* semaphore operation */
		short          sem_flg;
	}

**my_msg**
è una struttura che contiene il messaggio da inviare tramite la coda dei messaggi.
	
	typedef struct my_msg {
		long mtype;
		int mtext;
	}my_msg;

+ *mtype*: indica la priorità del messaggio, nel nostro caso è stato settato a 1.
+ *mtext*: questo è il messaggio vero e proprio.

**semun**
Questa struttura viene passata per argomento alla s.c. semctl, per creare il semaforo e settarlo la prima volta.
	
	typedef union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} semun;

###System Call:

Per ottenere l’esecuzione di istruzioni privilegiate, un programma di utente deve chiamare una System Call:

1.  invio di un’interruzione software al S.O.

2.  Salvataggio dello stato (PC, registri, bit di modo, etc.) del programma chiamante e trasferimento del controllo al  S.O.

3. Il S.O. esegue in modalità kernel l’operazione richiesta

4. al termine dell’operazione, il controllo ritorna al programma chiamante (ritorno al modo user )
Le system call di Unix sono attivabili attraverso funzioni di librerie C standard, sono quindi funzioni di libreria che chiamano le s.c. corrispondenti. Queste chiamate in generale restituiscono un valore positivo se tutto è andato a buon fine, in caso contrario restituiscono -1.

**fork**
La funzione fork() consente a un processo di generare un processo figlio:
Padre e figlio condividono lo stesso codice, il figlio eredita quindi una copia dei dati (di utente e di kernel) del padre.
La fork non richiede parametri e restituisce il PID del processo figlio, mentre al figlio questa variabile resterà a 0, in questo modo si può distinguere nel codice una parte che deve essere eseguita dal figlio e l’altra dal padre.

**execl**
La funzione execl() consente di fare eseguire un codice diverso al processo figlio. 
+ *Prototipo:*	int execl(const char *path, const char *arg, ...)
+ *path:* pathname di dove si trova il file eseguibile
+ *arg:* parametri da passare alla funzione

**exit**
questa system call termina il processo che la chiama, prende come parametro il valore di ritorno che si vuole restituire al padre. Il padre riceve il segnale SIGCHLD

**wait**
La chiamata di sistema wait si utilizza per lasciare il processo padre in attesa della terminazione dei figli.
Se il processo termina, quando il padre è già terminato, il processo figlio passa nello stato zombie. In questo caso sarà il processo INIT ad “adottarlo” e a procedere alla sua chiusura. Quindi in caso di presenza di figli il processo che chiama la wait si sospende.  Nel caso in cui non ci siano la wait termina immediatamente.
+ *Prototipo:*	pid_t wait(int *status)
+ *status:* è un puntatore ad un intero, in cui figlio scriverà il suo valore di ritorno, che sarebbe il valore contenuto in exit().

**shmget**
Si utilizza per creare una nuova memoria condivisa, o accedere a una memoria condivisa esistente.
+ *Prototipo:*	int shmget ( key_t key, int size, int shmflg )
+ *key:* indica la chiave di accesso alla memoria condivisa a cui si vuole accedere
+ *size:* grandezza della memoria condivisa
+ *shmflg:* indica i permessi o altri flag
+ *valore di ritorno:* shmid, identificatore di quella memoria condivisa


**shmat**
Con questa s.c. si unisce al proprio spazio di indirizzamento la memoria condivisa identificata dal shmid, cosi da poterla utilizzare.
+ *Protoipo:* int shmat ( int shmid, char *shmaddr, int shmflg)
+ *shmaddr:* specifica l’indirizzo, se uguale a NULL il processo usa il primo libero che trova
+ *valore di ritorno:* puntatore alla memoria condivisa

**semget**
Si utilizza per creare nuovi semafori o per accedere a esistenti
+ *Prototipo:* int semget ( key_t key, int nsems, int semflg)
+ *key:* chiave del semaforo a cui stiamo accedendo
+ *nsems:* specifica il numero di semafori che stiamo creando/accedendo
+ *semflg:* SEMPERM, variabile per i permessi
+ *valore di ritorno:* semid, identificatore del semaforo

**semop**
Si utilizza per fare operazioni sul semaforo (incrementare/decrementare)
+ *Prototipo:* int semop(int semid, struct sembuf *sops, unsigned nsops)
+ *semid:* indica su quale semaforo fare l’operazione
+ *sops:* è una struttura con all’interno le proprietà del semaforo.

**semctl**
Si utilizza per settare un semaforo quando viene creato, o per eliminarlo.
+ *Prototipo:* int semctl ( int semid , int semnum , int cmd, . . . )
+ *semnum:* ID dello specifico semaforo su cui operare, perchè semid può essere un vettore di semafori
+ *cmd:* comando da eseguire. Noi usiamo SETVAL e IPC_RMID, il primo si usa per resettare un semaforo con il valore passato per argomento (sarebbero i puntini); il secondo per eliminare il semaforo.

**msgget**
Serve ad aprire/creare una coda di messaggi
+ *Prototipo:* int msgget(key_t key, int msgflg)
+ *key:* chiave della coda. 
+ *valore di ritorno:* l'identificatore della coda. 
+ *msgflag:* 0666 | IPC_CREAT | IPC_EXCL cioè permessi a tutti, se non esiste creala, se esiste non fare niente.

**msgsnd / msgrcv**
Serve ad inviare/ricevere un messaggio
+ *Prototipo:*	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg)
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg)
+ *msqid:* identificatore della coda dei messaggi
+ *msgp:* messaggio da inviare o il buffer su cui scrivere quello ricevuto, nel nostro caso inviamo la struttura my_msg
+ *msgsz:* dimensione in byte del messaggio
+ *valore di ritorno della rcv:* grandeza del buffer

**msgctl**
Serve a per modificare le impostazioni della coda dei messaggi.
+ *Prototipo:* int msgctl(int msqid, int cmd, struct msqid_ds *buf)
+ *cmd:* con il parametro IPC_RMID si può eliminare la coda dei messaggi, questo ha effetto immediato ed eventuali messaggi contenuti sono persi.

**getpid**
Ritorna il pid (process ID) del processo che usa questa s.c.

###Funzioni di supporto:
**waitSem**
sei il valore del semaforo è diverso da 0, lo decrementa di uno, e va avanti sul codice. In caso contrario rimane in attesa che il valore venga incrementato di uno.
	
	int waitSem(int semid) {
		struct sembuf wait_buf;
		wait _ buf.sem _ num = 0;
		wait _ buf.sem _ op = -1;
		wait _ buf.sem _ flg = 0;
		if((semop(semid, &wait _ buf, 1)) <0) {
			perror( waitSem fallita );
			return(-1);
		}
		return(0);
	}
	
**signalSem**
Incrementa il valore del semaforo di uno e continua nel codice.
	
	int signalSem(int semid) {
		struct sembuf signal_buf;
		signal_buf.sem_num = 0;
		signal_buf.sem_op = 1;
		signal_buf.sem_flg = 0;
		if(semop(semid, &signal_buf, 1) <0) {
			perror("signalSem fallita");
			return(-1);
		}
		return(0);
	}

**errno**
Numero dell’ultimo errore della s.c.

**perror**
Visualizza nello standard output gli errori dell’ultima s.c..
E’ buona norma utilizzarlo quando si è sicuri che la s.c. ha dato un errore perchè in caso contrario riceve l’ultimo errore che si è verificato, che potrebbe non essere della s.c. che stiamo controllando.
+ *Prototipo:*void perror(const char * s)
+ *s:* una stringa che viene concatenata prima dell’errore, utilizzata per identificare ciò che ha fatto errore


####Makefile
Il makefile è un file con dentro una ricetta da eseguire sulla shell. Attraverso il comando make questo viene letto e eseguito. Di solito viene utilizzato per compilare dei progetti, quando hanno più file e librerie.
La prima parola prima dei due punti indica il nome del progetto, quello che c’è alla destra sono i componenti che servono per farlo. Se questi sono dei nomi file verifica se ci sono, in caso contrario va a crearli singolarmente con le sotto ricette. A ogni passo se ha tutti gli elementi per fare la ricetta esegue i comandi scritto sotto dopo il tab.

	oggetto: bin src/tutto.o starter bin/sem_mem bin/benzinaio bin/macchina
	
	bin: 
	mkdir bin
	
	src/tutto.o: src/tutto.c src/tutto.h
		cd src && gcc -c tutto.c
	
	starter: src/starter.c src/tutto.o
		cd src && gcc starter.c tutto.o -o ../starter
	
	bin/sem_mem: src/sem_mem.c src/tutto.o
		cd src && gcc sem_mem.c tutto.o -o ../bin/sem_mem
	
	bin/benzinaio: src/benzinaio.c src/tutto.o
		cd src && gcc benzinaio.c tutto.o -o ../bin/benzinaio
		
	bin/macchina: src/macchina.c src/tutto.o
		cd src && gcc macchina.c tutto.o -o ../bin/macchina
