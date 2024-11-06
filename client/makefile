client: client.o send.o receive.o connection.o gui.o
	gcc client.o utility/send.o utility/receive.o utility/connection.o utility/gui.o -o client

client.o:
	 gcc -c client.c -o client.o

send.o:
	 gcc -c utility/send.c -o utility/send.o

receive.o:
	 gcc -c utility/receive.c -o utility/receive.o

connection.o:
	 gcc -c utility/connection.c -o utility/connection.o

gui.o:
	 gcc -c utility/gui.c -o utility/gui.o

run_client: client
	./client 127.0.0.1 10000 1

clean:
	 rm -f client client.o utility/gui.o utility/send.o utility/receive.o utility/connection.o
	 