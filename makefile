client:
	gcc client.c -o client

run_client: client
	./client 127.0.0.1 1000

server: 
	gcc -pthreads server.c -o server

run_server: server
	./server 10000

clean:
	rm -f server.o client.o lient server

