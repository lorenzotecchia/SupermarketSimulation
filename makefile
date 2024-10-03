client: client.o
	gcc client.o -o client # Runs third

client.o:
	gcc -c client.c -o client.o # Runs second

run_client: client
	./client 127.0.0.1 10000


server: server.o
	gcc server.o -o server # Runs third

server.o: 
	gcc -c -pthreads server.c -o server.o # Runs second

run_server: server
	./server 10000

clean:
	rm -f client client.o server.o server
