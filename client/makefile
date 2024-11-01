client: client.o send.o receive.o connection.o
    gcc client.o send.o receive.o connection.o -o client

client.o: client.c connection.h
    gcc -c client.c -o client.o

send.o: send.c connection.h
    gcc -c send.c -o send.o

receive.o: receive.c connection.h
    gcc -c receive.c -o receive.o

connection.o: connection.c connection.h
    gcc -c connection.c -o connection.o

run_client: client
    ./client 127.0.0.1 10000

clean:
    rm -f client client.o send.o receive.o connection.o