#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 10
#define BUFFER_SIZE 1024

typedef struct pthread_arg_t {
  int new_socket_fd;
  struct sockaddr_in client_address;
} pthread_arg_t;

/* Thread routine to serve connection to client. */
void *pthread_routine(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

int main(int argc, char *argv[]) {
  int port, socket_fd, new_socket_fd;
  struct sockaddr_in address;
  pthread_attr_t pthread_attr;
  pthread_arg_t *pthread_arg;
  pthread_t pthread;
  socklen_t client_address_len;

  /* Get port from command line arguments or stdin. */
  port = argc > 1 ? atoi(argv[1]) : 0;
  if (!port) {
    printf("Enter Port: ");
    scanf("%d", &port);
  }

  /* Initialise IPv4 address. */
  memset(&address, 0, sizeof address);
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;

  /* Create TCP socket. */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Bind address to socket. */
  if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
    perror("bind");
    exit(1);
  }

  /* Listen on socket. */
  if (listen(socket_fd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  /* Assign signal handlers to signals. */
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(1);
  }
  if (signal(SIGTERM, signal_handler) == SIG_ERR) {
    perror("signal");
    exit(1);
  }
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    perror("signal");
    exit(1);
  }

  /* Initialise pthread attribute to create detached threads. */
  if (pthread_attr_init(&pthread_attr) != 0) {
    perror("pthread_attr_init");
    exit(1);
  }
  if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) !=
      0) {
    perror("pthread_attr_setdetachstate");
    exit(1);
  }

  while (1) {
    /* Create pthread argument for each connection to client. */
    pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
    if (!pthread_arg) {
      perror("malloc");
      continue;
    }

    /* Accept connection to client. */
    client_address_len = sizeof pthread_arg->client_address;
    new_socket_fd =
        accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address,
               &client_address_len);
    if (new_socket_fd == -1) {
      perror("accept");
      free(pthread_arg);
      continue;
    }

    /* Initialise pthread argument. */
    pthread_arg->new_socket_fd = new_socket_fd;

    /* Create thread to serve connection to client. */
    if (pthread_create(&pthread, &pthread_attr, pthread_routine,
                       (void *)pthread_arg) != 0) {
      perror("pthread_create");
      free(pthread_arg);
      continue;
    }
  }

  return 0;
}

void *pthread_routine(void *arg) {
  pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
  int new_socket_fd = pthread_arg->new_socket_fd;
  char buffer[BUFFER_SIZE] = {0};

  free(arg); // Free the allocated argument memory

  /* Receive a message from the client */
  if (read(new_socket_fd, buffer, BUFFER_SIZE) == -1) {
    perror("read");
    close(new_socket_fd);
    return NULL;
  }

  printf("Received message from client: %s\n", buffer);

  /* Echo the message back to the client */

  if (write(new_socket_fd, "mammt", strlen("mammt")) == -1) {
    perror("write");
  }

  /* Close the connection */
  close(new_socket_fd);
  return NULL;
}

void signal_handler(int signal_number) {
  /* Exit cleanup can be implemented here if needed */
  exit(0);
}
