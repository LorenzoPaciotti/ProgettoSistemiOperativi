ASSIGNMENT I

TESTO DELL'ESERCIZIO:
Si realizzi uno script bash che implementi una versione testuale del seguente gioco.
Il gioco si svolge cosi`:
Una volta lanciato il comando, questo seleziona una riga da un file di testo (il pathname del file viene fornito come parametro del comando).
Ogni riga del file deve contenere tre informazioni: una domanda, la risposta ed in numero massimo di errori N ammessi per quella specifica domanda.
Si assuma che la risposta sia sempre composta da una unica parola.
Fatto questo, lo script scrive la domanda in modo che l'utente possa leggerla.
Poi attende che l'utente digiti una risposta. Se la risposta e` corretta, allora ci si congratula. Se la risposta e` sbagliata si comunica che c'e` stato un errore, il numero di errori che possono ancora essere commessi viene decrementato, e lo script ripresenta la stessa domanda attendendo un'altra risposta.
        
L'utente vince se fornisce la risposta corretta entro N tentativi, altrimenti perde.
        
Il file usato come argomento deve essere un file di testo generato a priori. Il suo contenuto puo` essere organizzato come preferite. [SUGG.: separate domanda, risposta e numero con un carattere "speciale" (ad esempio '/')]
Una volta terminato un gioco, lo script chiede se si vuole giocare ancora. In caso affermativo tutto si ripete (ma ovviamente selezionando una riga diversa da quelle gia` usate).
Lo script dovrebbe controllare se la risposta data dall'utente coincida con quella corretta.
Potete gestire come preferite, distinguendole o accomunandole, le maiuscole e le minuscole.

In caso di aspetti non specificati nel testo dell'esercizio o in ogni caso in cui si possano configurare diverse scelte alternative nella progettazione della soluzione, effettuate voi le scelte che vi sembrano piu` ragionevoli (giustificandole opportunamente nei commenti al codice).


Relazione
Questo programma serve per fare un gioco di tipo quiz. Le domande risiedono in un file insieme alle relative risposte e al numero di tentativi, separate dal carattere speciale “:”. Questo file deve essere passato per parametro al programma nel momento dell’avvio. Appena partito il programma prima controlla se il numero degli argomenti è giusto e se il nome del file esiste. Dopo di che crea un file temporaneo con le righe mischiate a caso che verrà scorso dall’alto verso il basso con i comandi head e tail. A ogni ciclo accede a una riga del file e attraverso il comando cat prende la domanda, la risposta e il numeri di tentativi mettendoli in variabili di appoggio. Dopo di che il programma scrive a schermo la domanda, verifica se la risposta dell’utente è giusta, se è sbagliata lo fa ritentare per il numero di volte scritto nel file. A ogni risposta viene chiesto al giocatore se vuole continuare, se risponde negativamente allora programma elimina il file temporaneo creato all’inizio e termina.



Comandi usati:
-eq -lt -ne sono operatori di confronto per numeri interi rispettivamente uguale, minore_uguale, non_uguale.
sort mischia le righe di un file con -R lo fa in modo random 
cut con i parametri -d -f viene utilizzato per spezzare la stringa rispetto un carattere speciale.
cat stampare sullo standar-output un file passato per parametro
wc conta il numero di righe di un file con il parametro -l
shopt serve a cambiare la modalita della shell ad esempio disabilitare il case sensitive -s set -u unset
head -n k significa che stampa le prime k righe del file
tail -n k significa che stampa le ultime k righe del file 
let comando per fare operazioni matematiche
read legge dallo standard-input

Concetti generali:
<, > sono utilizzati per redirizionare lo standard output e lo standard input di un comando da o verso un file.
in linux un nome di un file che comincia con il punto significa che è nascosto
i comandi head e tail usati in pipe permette di selezionare una riga da un file di testo
con il $ si accede al valore di una variabile.
i confronti fra stringhe devono essere messi fra parentesi quadra
$# indica il numero degli argomenti passati al programma, mentre $1,$2,$3...$n, sono i valori di questi argomenti.
nell’assignment c’è un errore, settiamo sia all’inizio che alla fine il case unsensitive, questo però non provoca un errori nel programma, perchè alla fine dell’esecuzione dello script la shell ritorna alle impostazioni di default, rimettendo quindi il case sensitive.
