#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "models.h"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100
#define MAX_CASHIERS 20

typedef struct pthread_arg_t {
    Supermercato *supermercato;
    int new_socket_fd;
    struct sockaddr_in client_address;
} pthread_arg_t;

void setup_server_socket(int server_port, int *socket_fd);
void accept_connections(int socket_fd, Supermercato *supermercato);
void signal_handler(int signal_number);
void *client_handler(void *arg);
