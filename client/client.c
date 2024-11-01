#include "../include/connection.h"
#include "../include/receive.h"
#include "../include/send.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_NAME_LEN_MAX 255
#define BUFFER_SIZE 1024

void print_welcome_message();
void get_server_info(int argc, char *argv[], char *server_name,
                     int *server_port, int *num_clients);
int connect_to_server(const char *server_name, int server_port);
void guessing_routine(int socket_fd);
void close_resources(int socket_fd);

int main(int argc, char *argv[]) {
  char server_name[SERVER_NAME_LEN_MAX + 1] = {0};
  int server_port, socket_fd, num_clients;
  char buffer[BUFFER_SIZE] = {0};

  print_welcome_message();

  /* Get server name, port, and number of clients from command line arguments or
   * stdin. */
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
      close(socket_fd);
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
