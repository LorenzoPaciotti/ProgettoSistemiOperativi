ASSIGNMENT 3:
--------------

SCADENZA DELLA CONSEGNA:
      Consegnare via email (vedi dopo) ENTRO il 20 gennaio 2013

MODALITA` DI CONSEGNA:
      La consegna deve avvenire spedendo un archivio compresso. Se il vostro numero
      di matricola e` 123456 allora l'archivio si avra`  nome 123456.tgz (ottenuto con
      i comandi tar e gzip) e conterra` una directory di nome 123456.
      La directory 123456 deve contenere (almeno):

    * Un file di testo avente nome 123456.txt in cui siano indicati nome, cognome,
      matricola dello studente.
    * Il/i file con il codice C, debitamente commentato, della soluzione dell'esercizio.
    * Il Makefile.
    * Una breve descrizione della soluzione e delle scelte effettuate nella
      realizzazione del progetto (va bene se in formato testo).



TESTO DELL'ESERCIZIO:
        Si realizzi in C il codice relativo ai processi server e client che operino in questo modo:
        Il processo server fornisce un servizio "informazioni" sui file presenti nel "suo" filesystem.
        In pratica il servero opera cosi':
        accetta connessione da un generico client.
        Il client invia al server un pathname. il Server verifica se localmente al server esiste
        un file identificato da quel pathname. Se il file non esiste comunica al client che
        il fine non c'e'. Se il file esiste, restituisce al client delle statistiche sul file, ovvero:
        dimensione, data di creazione, di ulitma modifica, uid dell'utente proprietario.

        Tutte le trasmissioni devono avvenire tramite socket.

        Il server deve poter servire contemporaneamente piu' client.

        Il server termina quando un client gli spedisce la stringa "+FINE+" (in tal caso,
        prima di terminare restituisce al client la stringa "CIAO").



In caso di aspetti non specificati nel testo dell'esercizio o in ogni caso in cui si possano
configurare diverse scelte alternative nella progettazione della soluzione, effettuate voi
le scelte che vi sembrano piu` ragionevoli (giustificandole opportunamente nei commenti al codice). 


ALTRE INDICAZIONI GENERALI:
- realizzare il progetto in C in ambiente UNIX/Linux;
- il codice deve essere indentato in modo chiaro ed debitamente commentato;
- si richiede una adeguata gestione degli errori e delle anomalie (es. fallimento 
  nella chiamata ad una system call), possibilmente con una messaggistica di errore 
  che informi l'utente su eventuali anomalie accadute a tempo di esecuzione;
  (Possibilmente: se l'errore impone di terminare l'esecuzione, si devono prima 
  disallocare le risorse allocate.)
- il progetto deve essere corredato da un Makefile che permetta la compilazione
  e la generazione del/degli eseguibile/i;
- si eviti la generazione di processi zombie e il deadlock;
- per tutti i processi che terminano e tutte le risorse e le strutture di
  comunicazione utilizzare dai processi devono essere de-allocate; Nessun processo
  deve essere ucciso volontariamente da un altro processo, ma deve sempre terminare
  normalmente la sua esecuzione;
- al progetto deve essere allegata una (breve!!) relazione in formato testo (.txt)
  In cui siano descritte:
  --- le scelte di progetto che avete effettuato
  --- descrizione dei comandi con le eventuali opzioni da utilizzare per eseguire
      gli eseguibili;



SI RICORDA CHE:
La consegna della soluzione di questo esercizio e' necessaria per poter sostenere la
prova d'esame.
La valutazione terra' conto della correttezza e della completezza della soluzione
rispetto ai requisiti richiesti.
(NB. anche le soluzioni parziali verranno valutate).


