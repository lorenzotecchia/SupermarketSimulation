#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct pthread_arg_t {
  int new_socket_fd;
  struct sockaddr_in client_address;
} pthread_arg_t;

void *pthread_routine(void *arg);
void signal_handler(int signal_number);

int main(int argc, char *argv[]) {
  int socket_fd, new_socket_fd;
  struct sockaddr_in server_address, client_address;
  socklen_t client_address_len;
  int server_port;
  pthread_t pthread;
  pthread_attr_t pthread_attr;
  pthread_arg_t *pthread_arg;

  /* Set signal handler for Ctrl-C to gracefully shutdown the server */
  signal(SIGINT, signal_handler);

  /* Get server port from command line arguments or stdin. */
  server_port = argc > 1 ? atoi(argv[1]) : 0;
  if (!server_port) {
    printf("Enter Port: ");
    scanf("%d", &server_port);
  }

  /* Initialise IPv4 server address */
  memset(&server_address, 0, sizeof server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Create TCP socket */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Bind to socket */
  if (bind(socket_fd, (struct sockaddr *)&server_address,
           sizeof server_address) == -1) {
    perror("bind");
    exit(1);
  }

  /* Listen on socket */
  if (listen(socket_fd, 5) == -1) {
    perror("listen");
    exit(1);
  }

  /* Initialize pthread attribute to create detached threads */
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
  int min, max, guess, target;

  free(arg); // Free the allocated argument memory

  /* Receive the interval from the client */
  if (read(new_socket_fd, buffer, BUFFER_SIZE) == -1) {
    perror("read");
    close(new_socket_fd);
    return NULL;
  }

  sscanf(buffer, "%d %d", &min, &max);
  printf("Received interval: %d - %d\n", min, max);

  /* Generate a random number within the range */
  srand(time(NULL));
  target = rand() % (max - min + 1) + min;
  printf("Generated number: %d\n", target);

  while (1) {
    /* Clear the buffer and receive a guess from the client */
    memset(buffer, 0, BUFFER_SIZE);
    if (read(new_socket_fd, buffer, BUFFER_SIZE) == -1) {
      perror("read");
      close(new_socket_fd);
      return NULL;
    }

    guess = atoi(buffer);
    printf("Received guess: %d\n", guess);

    /* Compare the guess with the target */
    if (guess < target) {
      strcpy(buffer, "Too low");
    } else if (guess > target) {
      strcpy(buffer, "Too high");
    } else {
      strcpy(buffer, "Correct");
      if (write(new_socket_fd, buffer, strlen(buffer)) == -1) {
        perror("write");
      }
      break;
    }

    /* Send response to the client */
    if (write(new_socket_fd, buffer, strlen(buffer)) == -1) {
      perror("write");
      close(new_socket_fd);
      return NULL;
    }
  }

  /* Close the connection */
  close(new_socket_fd);
  return NULL;
}

void signal_handler(int signal_number) {
  /* Exit cleanup can be implemented here if needed */
  exit(0);
}
