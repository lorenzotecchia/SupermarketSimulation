server: server.o supermercato.o cassa.o cliente.o
    gcc -pthread server.o supermercato.o cassa.o cliente.o -o server

server.o: server.c
    gcc -c -pthread server.c -o server.o

supermercato.o: models_impl/supermercato.c
    gcc -c -pthread models_impl/supermercato.c -o supermercato.o

cassa.o: models_impl/cassa.c
    gcc -c -pthread models_impl/cassa.c -o cassa.o

cliente.o: models_impl/cliente.c
    gcc -c -pthread models_impl/cliente.c -o cliente.o

clean:
    rm -f server.o server_test.o supermercato.o cassa.o cliente.o server server_test
