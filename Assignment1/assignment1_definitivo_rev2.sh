#!/bin/bash

if [ $# -eq 0 ]; then
	echo "parametro mancante."
	echo "uso:"
	echo "assigment.sh <path file domande>"
	gioco=false
else
	gioco=true
	v=1 #variabile che indica il numero di riga da scegliere
	sort -R $1 > .file.tmp #crea un file temporaneo con le righe mescolate
	numRigheFile=`cat $1 | wc -l` #salva il  numero righe del file passato dall'utente
	shopt -s nocasematch  #disattiva il case-sensitive nei confronti fra stringhe

	while [ $gioco == true ] && [ $v -lt $numRigheFile ]; #controllo se il flag gioco è ancora a true e il file con le domande non è terminato
	do
		line=`head -n $v .file.tmp | tail -n1` #seleziona la v-esima riga
		tent=`echo $line | cut -f 3 -d ':'` #legge dal file il num di tentativi
		var=`echo $line | cut -f 2 -d ':'` #prende dal file la risposta corretta
		domanda=`echo $line | cut -f 1 -d ':'` #stampa la domanda
		echo "$domanda	(numero di tentativi rimasti $tent)"
		while [ $tent -ne 0 ];
		do 
			read risp #prende la mia risposta	

			if [[ $var = $risp ]]; then #controlla la risposta
				echo "bravissimissimo, la risposta è corretta"
				tent=0 #per uscire dal while
			else
				echo "ignorante, riprova"
				let "tent -= 1" #decrementa tent 
				echo "$domanda	(numero di tentativi rimasti $tent)"
			fi
		done

		echo "vuoi rigiocare? s per rigiocare]"
		read controllo
		if [[  $controllo = "s"  ]]; then 
			let "v += 1" #incrementa v così da scegliere al prossimo giro una riga diversa
		else 
			gioco=false
		fi
	done
	shopt -s nocasematch  #rinserisce il case-sensitive nei confronti fra stringhe
	rm .file.tmp #elimina il file temporaneo con le domani messe in ordine casuale
fi
