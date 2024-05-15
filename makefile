client: client.o
	cc client.o -o client # Runs third

client.o:
	cc -c client.c -o client.o # Runs second

run_client: client
	./client


server: server.o
	cc server.o -o server # Runs third

server.o: 
	cc -c server.c -o server.o # Runs second

run_server: server
	./server

clean:
	rm -f client client.o server.o server
