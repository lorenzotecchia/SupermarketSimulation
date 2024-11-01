client: client.o send.o receive.o
	gcc client.o send.o receive.o -o client

client.o: client.c
	gcc -c client.c -o client.o

send.o: send.c
	gcc -c send.c -o send.o

receive.o: receive.c
	gcc -c receive.c -o receive.o

run_client: client
	./client 127.0.0.1 10000

clean:
	rm -f client client.o send.o receive.o