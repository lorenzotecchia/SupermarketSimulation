#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "send.h"
#include "receive.h"

#define SERVER_NAME_LEN_MAX 255
#define BUFFER_SIZE 1024

void print_welcome_message();
void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients);
int connect_to_server(const char *server_name, int server_port);
void guessing_routine(int socket_fd);
void close_resources(int socket_fd);

int main(int argc, char *argv[]) {
  char server_name[SERVER_NAME_LEN_MAX + 1] = {0};
  int server_port, socket_fd, num_clients;
  char buffer[BUFFER_SIZE] = {0};

  print_welcome_message();

  /* Get server name, port, and number of clients from command line arguments or stdin. */
  get_server_info(argc, argv, server_name, &server_port, &num_clients);

  for (int i = 0; i < num_clients; i++) {
    if (fork() == 0) {
      /* Connect to server */
      socket_fd = connect_to_server(server_name, server_port);

      /* Send interval to the server */
      send_interval_to_server(socket_fd);

      /* Receive and print the response from the server */
      receive_message_from_server(socket_fd, buffer);
      printf("Server: %s\n", buffer);

      /* Start guessing the number */
      guessing_routine(socket_fd);

      /* Close resources */
      close_resources(socket_fd);
      exit(0);
    }
  }

  /* Wait for all child processes to finish */
  for (int i = 0; i < num_clients; i++) {
    wait(NULL);
  }

  return 0;
}

void print_welcome_message() {
    printf("\033[1;34m"); // Set text color to blue
    printf("********************************************\n");
    printf("*           Welcome to the Client!         *\n");
    printf("********************************************\n");
    printf("\033[0m"); // Reset text color
}

void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients) {
  if (argc > 1) {
    strncpy(server_name, argv[1], SERVER_NAME_LEN_MAX);
  } else {
    printf("Enter Server Name: ");
    scanf("%s", server_name);
  }
  *server_port = argc > 2 ? atoi(argv[2]) : 0;
  if (!*server_port) {
    printf("Enter Port: ");
    scanf("%d", server_port);
  }
  *num_clients = argc > 3 ? atoi(argv[3]) : 1;
  if (*num_clients <= 0) {
    printf("Enter Number of Clients: ");
    scanf("%d", num_clients);
  }
}

int connect_to_server(const char *server_name, int server_port) {
  int socket_fd;
  struct hostent *server_host;
  struct sockaddr_in server_address;

  /* Get server host from server name. */
  server_host = gethostbyname(server_name);
  if (!server_host) {
    fprintf(stderr, "Could not resolve host\n");
    exit(1);
  }

  /* Initialise server address */
  memset(&server_address, 0, sizeof server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

  /* Create TCP socket */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Connect to server */
  if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address) == -1) {
    perror("connect");
    close(socket_fd);
    exit(1);
  }

  return socket_fd;
}

void guessing_routine(int socket_fd) {
  char buffer[BUFFER_SIZE];
  int guess;

  while (1) {
    printf("Enter your guess: ");
    scanf("%d", &guess);

    sprintf(buffer, "%d", guess);
    if (write(socket_fd, buffer, strlen(buffer)) == -1) {
      perror("write");
      close(socket_fd);
      exit(1);
    }

    /* Receive response from the server */
    receive_message_from_server(socket_fd, buffer);
    printf("Server: %s\n", buffer);

    if (strcmp(buffer, "Correct!") == 0) {
      break;
    }
  }
}

void close_resources(int socket_fd) {
  close(socket_fd);
}