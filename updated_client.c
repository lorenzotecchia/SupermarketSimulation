#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_NAME_LEN_MAX 255
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  char server_name[SERVER_NAME_LEN_MAX + 1] = {0};
  int server_port, socket_fd;
  struct hostent *server_host;
  struct sockaddr_in server_address;
  char buffer[BUFFER_SIZE] = {0};

  /* Get server name and port from command line arguments or stdin. */
  if (argc > 1) {
    strncpy(server_name, argv[1], SERVER_NAME_LEN_MAX);
  } else {
    printf("Enter Server Name: ");
    scanf("%s", server_name);
  }
  server_port = argc > 2 ? atoi(argv[2]) : 0;
  if (!server_port) {
    printf("Enter Port: ");
    scanf("%d", &server_port);
  }

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
  memcpy(&server_address.sin_addr.s_addr, server_host->h_addr,
         server_host->h_length);

  /* Create TCP socket */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Connect to server */
  if (connect(socket_fd, (struct sockaddr *)&server_address,
              sizeof server_address) == -1) {
    perror("connect");
    close(socket_fd);
    exit(1);
  }

  /* Send interval to the server */
  int min, max;
  printf("Enter minimum number: ");
  scanf("%d", &min);
  printf("Enter maximum number: ");
  scanf("%d", &max);

  sprintf(buffer, "%d %d", min, max);
  if (write(socket_fd, buffer, strlen(buffer)) == -1) {
    perror("write");
    close(socket_fd);
    exit(1);
  }

  /* Receive and print the response from the server */
  memset(buffer, 0, BUFFER_SIZE);
  if (read(socket_fd, buffer, BUFFER_SIZE) == -1) {
    perror("read");
    close(socket_fd);
    exit(1);
  }
  printf("Server: %s\n", buffer);

  /* Start guessing the number */
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
    memset(buffer, 0, BUFFER_SIZE);
    if (read(socket_fd, buffer, BUFFER_SIZE) == -1) {
      perror("read");
      close(socket_fd);
      exit(1);
    }
    printf("Server: %s\n", buffer);

    if (strcmp(buffer, "Correct!") == 0) {
      break;
    }
  }

  close(socket_fd);
  return 0;
}
