client: client.o send.o receive.o connection.o
	 gcc client.o utility/send.o utility/receive.o utility/connection.o -o client

client.o:
	 gcc -c client.c -o client.o

send.o:
	 gcc -c utility/send.c -o utility/send.o

receive.o:
	 gcc -c utility/receive.c -o utility/receive.o

connection.o:
	 gcc -c utility/connection.c -o utility/connection.o

run_client: client
	 ./client 127.0.0.1 10000

clean:
	 rm -f client client.o utility/send.o utility/receive.o utility/connection.o
