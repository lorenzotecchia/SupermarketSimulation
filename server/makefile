server: server.o
	gcc server.o -o server # Runs third

server.o: 
	gcc -c -pthreads server.c -o server.o # Runs second

run_server: server
	./server 10000

clean:
	rm -f server.o server
