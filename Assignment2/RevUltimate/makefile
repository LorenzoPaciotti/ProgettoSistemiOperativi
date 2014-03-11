progetto: bin src/tutto.o starter bin/sem_mem bin/benzinaio bin/macchina 

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
