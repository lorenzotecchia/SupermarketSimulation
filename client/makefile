client: client.o receive.o connection.o gui.o
	gcc client.o client_models_impl/receive.o client_models_impl/connection.o client_models_impl/gui.o -o client

client.o:
	 gcc -c client.c -o client.o

receive.o:
	 gcc -c client_models_impl/receive.c -o client_models_impl/receive.o

connection.o:
	 gcc -c client_models_impl/connection.c -o client_models_impl/connection.o

gui.o:
	 gcc -c client_models_impl/gui.c -o client_models_impl/gui.o

run_client: client
	./client 127.0.0.1 10000 20

clean:
	rm -f client client.o client_models_impl/gui.o client_models_impl/receive.o client_models_impl/connection.o

