ASSIGNMENT 2:
--------------

SCADENZA DELLA CONSEGNA:
      Consegnare via email (vedi dopo) ENTRO il 15 dicembre 2012

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

    Si realizzi in C una soluzione della seguente versione del problema dei "due benzinai".

    Ad una stazione di rifornimento ci sono due pompe di benzina. Vi sono due benzinai
    che possono: aspettare le auto o operare ad una delle pompe.
    Ogni benzinaio lavora sempre alla stessa pompa.
    Le auto giungono alla stazione, se vi sono pompe libere si dirigono verso una di
    esse e si fermano ed il benzinaio addetto a quella pompa riempie il serbatoio dell'auto.
    Finito il rifornimento l'auto chiede il costo del pieno. Il benzinaio comunica
    l'ammontare della spesa. L'auto paga e il benzinario prende i soldi.
    Fatto cio` l'auto riparte. Il benzinaio porta i soldi alla cassa (che e` unica), li
    deposita e torna ad aspettare la prossima auto.
    Se non vi sono pompe libere l'auto aspetta che una pompa si liberi.
    Rifornita l'ultima auto i due benzinai devono dividersi l'incasso della giornata e
    "chiudere" la stazione.

    Risolvere il problema utilizzando i semafori per la sincronizzazione e la mutua esclusione.
    La comunicazione tra benzinaio e auto deve avvenire utilizzando o delle code di messaggi
    o delle memorie condivise (o entrambe, se preferite).

    Prevedere che vi sia un numero prefissato K di auto (specificato da una costante nel
    programma, oppure da un argomento passato sulla linea del comando, oppure...).



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


