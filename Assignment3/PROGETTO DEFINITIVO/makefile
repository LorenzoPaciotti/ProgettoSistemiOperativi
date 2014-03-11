progetto: src/libClientServer.o client server

src/libClientServer.o: src/libClientServer.c src/libClientServer.h
	cd src && gcc -g -c libClientServer.c 

client: src/client.c src/libClientServer.o
	cd src && gcc -g client.c libClientServer.o -o ../client

server: src/server.c src/libClientServer.o
	cd src && gcc -g server.c libClientServer.o -o ../server
