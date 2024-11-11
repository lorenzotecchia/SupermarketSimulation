# Obiettivo principale
server: server.o supermercato.o cassa.o cliente.o connection.o
	 gcc -pthread server.o supermercato.o cassa.o cliente.o connection.o -o server

# Compilazione dei file sorgente
server.o: 
	 gcc -c -pthread server.c -o server.o

connection.o:
	gcc -c -pthread server_models_impl/connection.c -o connection.o

supermercato.o: 
	 gcc -c -pthread server_models_impl/supermercato.c -o supermercato.o

cassa.o: 
	 gcc -c -pthread server_models_impl/cassa.c -o cassa.o

cliente.o: 
	 gcc -c -pthread server_models_impl/cliente.c -o cliente.o

# Pulizia dei file oggetto e dell'eseguibile
clean:
	 rm -f server.o supermercato.o cassa.o cliente.o server connection.o

run_server: server
	./server 10000 127.0.0.1 5
